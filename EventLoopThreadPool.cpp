//
// Created by Svain on 2022/8/15.
//

#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop, int num_threads) : loop_(loop), started_(false),
                                        num_threads_(num_threads), next_(0) {
    if (num_threads_ <= 0) {
        LOG << "Runtime Error: the thread number of event loop thread pool <= 0";
        abort();
    }
}

void EventLoopThreadPool::thread_pool_start() {
    assert(loop_->is_thread_in_loop());
    started_ = true;
    for (int i = 0; i < num_threads_; i++) {
        shared_ptr<EventLoopThread> thread(new EventLoopThread());
        threads_.push_back(thread);
        loops_.push_back(thread->start_loop());
    }
}

void EventLoopThreadPool::get_next_loop() {
    assert(loop_->is_thread_in_loop());
    assert(started_);
    EventLoop *loop = loop_;
    if (!loops_.empty()) {
        loop = loops_[next_];
        next_ = (next_ + 1) % num_threads_;
    }
    return loop;
}