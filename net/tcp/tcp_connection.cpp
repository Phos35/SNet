#include "tcp_connection.h"
#include "logger.h"   
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <random>

TCPConnection::TCPConnection(size_t id, EventLoop* event_loop, 
                             SocketPtr&& client_socket,
                             WorkerPool* pool)
:id_(id), loop_(event_loop), socket_(std::move(client_socket)), state_(State::CREATING),
 event_(Event(Event::OwnerType::CONNECTION,socket_->fd(), EPOLLIN)),
 worker_pool_(pool), recv_buffer_(65536),send_buffer_(65536),
 writing_(false), 
 timer_(Event::OwnerType::NONE, timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK), EPOLLIN),
 last_active_(Timestamp::now())
{
    // LOG_INFO << "\nTCPConnection " << id_ << " Creating\n"
    //          << "Manage fd: " << event_.fd() << "\n"
    //          << "owner loop: " << loop_->id() << "\n"
    //          << "current state: " << state_to_string(state_) << "\n"
    //          << "client addr: " << socket_->addr_str() << ":" << socket_->port() << "\n";
}

TCPConnection::~TCPConnection()
{
    LOG_TRACE << "TCPConnection " << id_
              << " deconstruct, state: " << state_to_string(state_);
}

void TCPConnection::create()
{
    // 设置事件相关的回调函数，并注册事件到事件循环中
    event_.set_weak_ptr(shared_from_this());
    // 注意此处传递this是合理的，因为在event中会通过weak_ptr判断连接是否存活
    event_.set_read_callback(std::bind(&TCPConnection::process_read, this));
    event_.set_write_callback(std::bind(&TCPConnection::process_write, this));
    event_.set_close_callback(std::bind(&TCPConnection::process_close, this));
    loop_->add_event(event_);

    // 设置timer的超时时间和超时处理函数
    set_timer();
    timer_.set_read_callback(std::bind(&TCPConnection::process_expire, this));
    loop_->add_event(timer_);

    // 更新状态
    state_ = State::CONNECTING;
    TCPConnection::SPtr ptr = shared_from_this();
    LOG_DEBUG << "TCPConnection " << id_ << " created, counter: " << ptr.use_count();
}

void TCPConnection::set_close_callback(const CloseCallBack &close_cb)
{
    close_callback_ = std::move(close_cb);
}

void TCPConnection::write(const std::string &data)
{
    loop_->run_in_loop(std::bind(&TCPConnection::write_in_loop, shared_from_this(), data));
}

void TCPConnection::write_in_loop(const std::string& data)
{
    // 更新活跃时间
    // last_active_ = Timestamp::now();

    LOG_DEBUG << "TCPConnection " << id_ << " first write";
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
        LOG_DEBUG << "TCPConnection " << id_ << " first write finished";
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
        LOG_DEBUG << "TCPconnection " << id_ << " close in loop";
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

    // 取消关注的事件和定时器
    loop_->del_event(event_);
    loop_->del_event(timer_);

    // 更新状态
    state_ = State::CLOSED;
    TCPConnection::SPtr ptr = shared_from_this();
    LOG_DEBUG << "TCPConnection " << id_ << " closed, counter: " << ptr.use_count();
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
    LOG_DEBUG << "TCPConnection " << id_ << " read";
    worker_pool_->add_task(shared_from_this());

    // 更新活跃时间
    // last_active_ = Timestamp::now();
}

void TCPConnection::process_write()
{
    // 更新活跃时间
    // last_active_ = Timestamp::now();

    LOG_DEBUG << "TCPConnection " << id_ << " after write";
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

void TCPConnection::process_expire()
{
    char buf[8] = {0};
    int read_size = read(timer_.fd(), buf, 8);
    if ((Timestamp::now() - last_active_).seconds() >= expire_interval_)
    {
        LOG_DEBUG << "TCPConnection " << id_ << " long time silent: "
                  << expire_interval_ << ", now closing";
        close_in_loop();
    }
}

void TCPConnection::set_timer()
{
    // 此处使用均匀分布的随机数，生成10分钟~30分钟内的长期不活跃检测时间，
    // 可以避免不活跃检测大量聚集在某个时间点进行而导致的性能下降
    std::random_device random_device;
    std::mt19937 engine(random_device());
    std::uniform_int_distribution<size_t> distribution(10 * 60 * 1000, 30 * 60 * 1000);

    // 设置超时间隔
    expire_interval_ = distribution(engine);

    // 设置定时器启动时间和重复启动的超时间隔
    Timestamp now = Timestamp::now();
    timespec start_time = {.tv_sec = now.seconds() + expire_interval_, .tv_nsec = 0};
    timespec interval = {.tv_sec = expire_interval_, .tv_nsec = 0};
    itimerspec timer_info = {.it_interval = interval, .it_value = start_time};
    timerfd_settime(timer_.fd(), TFD_TIMER_ABSTIME, &timer_info, nullptr);
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