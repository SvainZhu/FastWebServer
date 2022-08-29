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

void EventLoop::handle_read() {
    uint64_t one = 1;
    ssize_t n = readn(wakeup_fd_, &one, sizeof one);
    if (n != sizeof one) LOG << "EventLoop::handle_read() reads " << n << " bytes instead of 8";
    wakeup_channel_sptr_->set_events(EPOLLIN | EPOLLET);

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

void EventLoop::run_in_loop(functor &&cb) {
    if (is_thread_in_loop()) cb();
    else queue_in_loop(std::move(cb));
}

void EventLoop::queue_in_loop(functor &&cb) {
    {
        MutexLockGuard lock(mutex_lock_);
        pending_functors_.emplace_back(std::move(cb));
    }
    if (!is_thread_in_loop() || calling_pending_functors_) wakeup();
}

void EventLoop::loop() {
    assert(!looping_);
    assert(is_thread_in_loop());
    looping_ = true;
    quit_ = false;
    LOG << "EventLoop" << this << "start looping";
    vector<channel_sptr> result;
    while (!quit_) {
        result.clear();
        result = poller_->poll();
        event_handling_ = true;
        for (auto& item : result) item->handle_events();
        event_handling_ = false;
        do_pending_functors();
        poller_->handle_expired();
    }
    looping_ = false;
}

void EventLoop::do_pending_functors() {
    vector<functor> functor_list;
    calling_pending_functors_ = true;
    {
        MutexLockGuard lock(mutex_lock_);
        functor_list.swap(pending_functors_);
    }
    for (auto& item : functor_list) item();
    calling_pending_functors_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    if (!is_thread_in_loop()) wakeup();
}

bool EventLoop::is_thread_in_loop() const {
    return thread_id_ == CurrentThread::tid();
}

void EventLoop::shutdown(channel_sptr channel) {
    shutdown_writer(channel->get_fd());
}

void  EventLoop::update_poller(channel_sptr channel, int timeout) {
    poller_->modify_fd_in_epoll(channel, timeout);
}

void EventLoop::remove_from_poller(channel_sptr channel) {
    poller_->delete_fd_in_epoll(channel);
}

void EventLoop::add_to_poller(shared_ptr<Channel> channel, int timeout) {
    poller_->add_fd_in_epoll(channel, timeout);
}