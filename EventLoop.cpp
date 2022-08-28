//
// Created by Svain on 2022/8/9.
//

#include "EventLoop.h"

__thread EventLoop* event_loop_in_this_thread = 0;

int create_event_fd() {
    int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (event_fd < 0) {
        LOG << "Event fd failed!!!";
        abort();
    }
    return event_fd;
}

EventLoop::EventLoop() : looping_(false), poller_(new Epoll()), wakeup_fd_(create_event_fd()),
                        quit_(false), event_handling_(false), calling_pending_functors_(false),
                        thread_id_(CurrentThread::tid()), wakeup_channel_sptr_(new Channel(this, wakeup_fd_)){
    if (event_loop_in_this_thread) {
        LOG << "Another EventLoop " << event_loop_in_this_thread << " exist in this thread "
        << thread_id_;
    }
    else {
        event_loop_in_this_thread = this;
    }
    wakeup_channel_sptr_->set_events(EPOLLIN | EPOLLET);
    wakeup_channel_sptr_->set_read_handler(bind(&EventLoop::handle_read, this));
    wakeup_channel_sptr_->set_connect_handler(bind(&EventLoop::handle_connect, this));
    poller_->add_fd_in_epoll(wakeup_channel_sptr_, 0);
}

void EventLoop::handle_connect() {
    poller_->modify_fd_in_epoll(wakeup_fd_, wakeup_channel_sptr_, (EPOLLIN | EPOLLET | EPOLLONESHOT),
                                0);
    update_poller(wakeup_channel_sptr_, 0);
}

EventLoop::~EventLoop() {
    close(wakeup_fd_);
    event_loop_in_this_thread = NULL;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = writen(wakeup_fd_, (char*)(&one), sizeof one);
    if (n != sizeof one) {
        LOG << "EventLoop::wakeup() writes" << n << " bytes instead of 8";
    }
}