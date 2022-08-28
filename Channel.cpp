//
// Created by Svain on 2022/8/9.
//

#include "Channel.h"

Channel::Channel(EventLoop *event_loop) : event_loop_(event_loop), events_(0), last_events_(0), fd_(0) {}

Channel::Channel(EventLoop *event_loop, int fd) : event_loop_(event_loop), events_(0), last_events_(0), fd_(fd) {}

Channel::~Channel() {}

int Channel::get_fd() {
    return fd_;
}

void Channel::set_fd(int fd) {
    fd_ = fd;
}

void Channel::set_holder(shared_ptr<HttpData> holder) {
    holder_ = holder;
}

shared_ptr<HttpData> Channel::get_holder() {
    shared_ptr<HttpData> holder(holder_.lock());
    return holder;
}

void Channel::set_read_handler(CallBack &&read_handler) {
    read_handler_ = read_handler;
}

void Channel::set_write_handler(CallBack &&write_handler) {
    write_handler_ = write_handler;
}

void Channel::set_connect_handler(CallBack &&connect_handler) {
    connect_handler_ = connect_handler;
}

void Channel::set_error_handler(CallBack &&error_handler) {
    error_handler_ = error_handler;
}

void Channel::handle_events() {
    events_ = 0;
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
        events_ = 0;
        return;
    }
    if (revents_ & EPOLLERR) {
        if (error_handler_) error_handler_();
        events_ = 0;
        return;
    }
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        handle_read();
    }
    if (revents_ & EPOLLOUT) {
        handle_write();
    }
    Channel::handle_connect();
}

void Channel::handle_read() {
    if (read_handler_) read_handler_();
}

void Channel::handle_write() {
    if (write_handler_) write_handler_();
}

void Channel::handle_connect() {
    if (connect_handler_) connect_handler_();
}

void Channel::set_events(__uint32_t events) {
    events_ = events
}

void Channel::set_revents(__uint32_t revents) {
    revents_ = revents;
}
bool Channel::equal_and_update_last_events() {
    bool result = events_ == last_events_;
    last_events_ = events_;
    return result;
}

__uint32_t Channel::get_events() {
    return events_;
}

__uint32_t Channel::get_last_events() {
    return last_events_;
}