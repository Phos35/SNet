#include "tcp_connection.h"
#include "logger.h"   
#include <unistd.h>

TCPConnection::TCPConnection(size_t id, EventLoop* event_loop, SocketPtr&& client_socket)
:id_(id), loop_(event_loop), socket_(std::move(client_socket)), 
 event_(Event(Event::OwnerType::CONNECTION,socket_->fd(), EPOLLIN)), state_(State::CREATING)
{
    LOG_INFO << "\nTCPConnection " << id_ << " Creating\n"
             << "Manage fd: " << event_.fd() << "\n"
             << "owner loop: " << loop_->id() << "\n"
             << "current state: " << state_to_string(state_) << "\n"
             << "client addr: " << socket_->addr_str() << ":" << socket_->port() << "\n";
}

TCPConnection::~TCPConnection()
{
    LOG_TRACE << "TCPConnection " << id_ << " deconstruct, state: " << state_to_string(state_);
}

void TCPConnection::create()
{
    // 设置事件相关的回调函数，并注册事件到事件循环中
    // 注意此处传递this是合理的，因为在event中会通过weak_ptr判断连接是否存活
    event_.set_weak_ptr(shared_from_this());
    event_.set_read_callback(std::bind(&TCPConnection::process_read, this));
    loop_->add_event(event_);

    // 更新状态
    state_ = State::CONNECTING;
    LOG_TRACE << "TCPConnection " << id_ << " created";
}

void TCPConnection::set_msg_callback(const MsgCallBack& msg_cb)
{
    msg_callback_ = std::move(msg_cb);
}

void TCPConnection::set_close_callback(const CloseCallBack &close_cb)
{
    close_callback_ = std::move(close_cb);
}

void TCPConnection::write(const std::string &data)
{
    write(data.c_str(), data.length());
}

void TCPConnection::write(const char *data, size_t length)
{
    loop_->run_in_loop(std::bind(&TCPConnection::write_in_loop, shared_from_this(), data, length));
}

// TODO 错误处理、单次写入未完成则启动写事件关注等
void TCPConnection::write_in_loop(const char* data, size_t length)
{
    int written_size = ::write(socket_->fd(), data, length);
    LOG_TRACE << "TCPConnection write data: " << written_size << " bytes";
}

void TCPConnection::close()
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

/// TODO 注意检验是否在事件循环内运行 -- 需要EventLoop修正访问权限
void TCPConnection::destroy()
{
    // 取消关注的事件
    loop_->del_event(event_);

    // 更新状态
    state_ = State::CLOSED;
    LOG_TRACE << "TCPConnection " << id_ << " closed";
}

// TODO 添加buffer进行读取
void TCPConnection::process_read()
{
    // TCP连接处于非连接状态，不进行读取
    if(state_ != State::CONNECTING)
    {
        LOG_WARN << "TCPConnection " << id_ << " recv read event in state: " << state_to_string(state_);
        return;
    }

    char buf[1024] = {0};
    int read_size = read(socket_->fd(), buf, 1024);
    if(read_size == -1)
    {
        // 发生错误
        if(errno != EAGAIN && errno != EAGAIN)
        {
            LOG_ERROR << "TCPConnection " << id_ << " read error: " << strerror(errno);
            return;
        }
    }

    // 对端关闭，则本端开始关闭
    if(read_size == 0)
    {
        close();
    }

    // 调用消息处理回调函数
    // TODO 将数据提交给Decoder
    if(!msg_callback_)
    {
        LOG_ERROR << "TCPConencion " << id_ << " MsgCallBack not set";
        return;
    }
    else
    {
        msg_callback_(shared_from_this(), buf);
    }
}

// TODO 在事件循环中续写未写完的数据
void TCPConnection::process_write()
{

}

// TODO 在明白除了read返回0外的对端关闭方式后再添加
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