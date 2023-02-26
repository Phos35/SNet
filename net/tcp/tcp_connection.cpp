#include "tcp_connection.h"
#include "logger.h"   
#include <unistd.h>

TCPConnection::TCPConnection(size_t id, EventLoop* event_loop, 
                             SocketPtr&& client_socket,
                             Decoder::Ptr decoder, 
                             Dispatcher::Ptr dispatcher,
                             WorkerPool* pool)
:id_(id), loop_(event_loop), socket_(std::move(client_socket)), state_(State::CREATING),
 event_(Event(Event::OwnerType::CONNECTION,socket_->fd(), EPOLLIN)), 
 decoder_(decoder), dispatcher_(dispatcher),
 worker_pool_(pool), recv_buffer_(65536),send_buffer_(65536),
 writing_(false)
{
    LOG_INFO << "\nTCPConnection " << id_ << " Creating\n"
             << "Manage fd: " << event_.fd() << "\n"
             << "owner loop: " << loop_->id() << "\n"
             << "current state: " << state_to_string(state_) << "\n"
             << "client addr: " << socket_->addr_str() << ":" << socket_->port() << "\n";
}

TCPConnection::~TCPConnection()
{
    LOG_TRACE << "TCPConnection " << id_
              << " deconstruct, state: " << state_to_string(state_);
}

void TCPConnection::create()
{
    // 设置事件相关的回调函数，并注册事件到事件循环中
    // 注意此处传递this是合理的，因为在event中会通过weak_ptr判断连接是否存活
    event_.set_weak_ptr(shared_from_this());
    event_.set_read_callback(std::bind(&TCPConnection::process_read, this));
    event_.set_write_callback(std::bind(&TCPConnection::process_write, this));
    event_.set_close_callback(std::bind(&TCPConnection::process_close, this));
    loop_->add_event(event_);

    // 更新状态
    state_ = State::CONNECTING;
    TCPConnection::SPtr ptr = shared_from_this();
    LOG_TRACE << "TCPConnection " << id_ << " created, counter: " << ptr.use_count();
}

void TCPConnection::set_close_callback(const CloseCallBack &close_cb)
{
    close_callback_ = std::move(close_cb);
}

void TCPConnection::write(const std::string &data)
{
    loop_->run_in_loop(std::bind(&TCPConnection::write_in_loop, this, data));
}

void TCPConnection::write_in_loop(const std::string& data)
{
    int written_size = ::write(socket_->fd(), data.data(), data.size());
    if(written_size == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        LOG_ERROR << "TCPConnection " << id_ << " write failed: "
                  << strerror(errno);
        return;
    }
    if(written_size == -1)
        written_size = 0;

    // 若数据写完，则一次完毕，无需后续写入
    if(written_size == data.size())
    {
        // 若处于优雅关闭连接的过程中，则关闭连接
        if(state_ == State::CLOSING)
        {
            destroy();
        }
        return;
    }

    // 若数据未写完，则
    // 1. 置writing为true
    writing_ = true;

    // 2. 关注写事件
    event_.focus_write();
    loop_->update_event(event_);

    // 3. 将剩余数据写入写出缓冲区
    send_buffer_.write(data.substr(written_size));
}

void TCPConnection::close()
{
    // 只在运行状态下调用in_loop的关闭函数
    if(state_ == State::CONNECTING)
    {
        loop_->run_in_loop(std::bind(&TCPConnection::close_in_loop, shared_from_this()));
    }
    // 其余状态提出警告
    else
    {
        LOG_WARN << "Trying to close TCPCoonection " << id_
                 << ", which state is: " << state_to_string(state_)
                 << ". Do nothing";
    }
}

void TCPConnection::close_in_loop()
{
    // 若连接在连接状态，则进行关闭
    if(state_ == State::CONNECTING)
    {
        if(!close_callback_)
        {
            LOG_ERROR << "TCPConnection " << id_ << "CloseCallBack Not Set";
            return;
        }
        else
        {
            // 更新状态
            state_ = State::CLOSING;
            close_callback_(shared_from_this());
        }
    }
    // 若处于正在关闭的状态，则提示告警并返回
    else if(state_ == State::CLOSING)
    {
        LOG_WARN << "TCPConnection " << id_ << " is still closing";
    }
    // 其它状态下的关闭视为非法行为，报告错误
    else
    {
        LOG_ERROR << "TCPConnection " << id_ << " recv an close request in state: " << state_to_string(state_);
    }
}

void TCPConnection::destroy()
{
    // 若目前仍有数据需要写出，则搁置关闭，由后续process_write写完数据后再调用
    if(writing_ == true)
        return;

    // 取消关注的事件
    loop_->del_event(event_);

    // 更新状态
    state_ = State::CLOSED;
    TCPConnection::SPtr ptr = shared_from_this();
    LOG_TRACE << "TCPConnection " << id_ << " closed, counter: " << ptr.use_count();
}

void TCPConnection::process_read()
{
    // TCP连接处于非连接状态，不进行读取
    if(state_ != State::CONNECTING)
    {
        TCPConnection::SPtr ptr = shared_from_this();
        char buf[1024] = {0};
        int read_size = read(socket_->fd(), buf, 1024);
        printf("Counter: %ld, read_size: %d\n", ptr.use_count(), read_size);
        LOG_WARN << "TCPConnection " << id_ << " recv read event in state: " << state_to_string(state_);
        return;
    }

    int read_size = recv_buffer_.read_from_fd(socket_->fd());
    if (read_size == -1)
    {
        // 若发生错误，则关闭连接
        if(errno != EAGAIN && errno != EAGAIN)
        {
            LOG_ERROR << "TCPConnection " << id_ << " read error: " << strerror(errno);
            close_in_loop();
        }
        return;
    }

    // 对端关闭，则本端开始关闭
    if(read_size == 0)
    {
        close_in_loop();
        return;
    }

    // 数据解析提交工作线程池
    worker_pool_->add_task(shared_from_this());
}

Message::SPtr TCPConnection::decode(const std::string& data)
{
    return decoder_->decode(data);
}

void TCPConnection::dispatch(const Message::SPtr& message)
{
    dispatcher_->dispatch(shared_from_this(), message);
}

void TCPConnection::process_write()
{
    // 写出数据
    int data_size = send_buffer_.readable();
    int written_size = ::write(socket_->fd(), send_buffer_.readable_area(), data_size);
    if(written_size == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        LOG_ERROR << "TCPConnection " << id_ << " process_read failed: "
                  << strerror(errno) << ". Now closing connection";
        writing_ = false;
        close_in_loop();
        return;
    }
    if(written_size == -1)
        written_size = 0;

    send_buffer_.release(written_size);
    // 若数据未写完，则继续等待写事件
    if(written_size < data_size)
    {
        return;
    }

    // 数据写完
    // 1. 置writing为false
    writing_ = false;

    // 2. 是否正在关闭中，若在关闭中，则调用destroy
    if(state_ == State::CLOSING)
    {
        destroy();
    }
    // 3.若未在关闭中，则取消写事件关注
    else
    {
        event_.unfocus_write();
        loop_->update_event(event_);
    }
}

void TCPConnection::process_close()
{

}

size_t TCPConnection::id()
{
    return id_;
}

EventLoop* TCPConnection::event_loop()
{
    return loop_;
}

std::string TCPConnection::addr()
{
    return socket_->addr_str();
}

uint16_t TCPConnection::port()
{
    return socket_->port();
}

TCPBuffer& TCPConnection::recv_buffer_ref()
{
    return recv_buffer_;
}

std::string TCPConnection::state_to_string(State s)
{
    switch(s)
    {
        case State::CREATING: return "CREATING"; break;
        case State::CONNECTING: return "CONNECTING"; break;
        case State::CLOSING: return "CLOSING"; break;
        case State::CLOSED: return "CLOSED"; break;
        default: return "UNKNOWN";break;
    }
}