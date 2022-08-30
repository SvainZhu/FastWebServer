//
// Created by Svain on 2022/8/15.
//

#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() : loop_(NULL), exiting_(false), mutex_lock_(), condition_(mutex_lock_),
                                     thread_(bind(&EventLoopThread::thread_func, this), "EventLoopThread") {}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != NULL) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::start_loop() {
    assert(!thread_.started());
    thread_.start();
    {
        // condition variable to wait the mutex lock
        MutexLockGuard lock(mutex_lock_);
        while (loop_ == NULL) condition_.wait();
    }
    return loop_;
}

void EventLoopThread::thread_func() {
    EventLoop loop;

    {
        MutexLockGuard lock(mutex_lock_);
        loop_ = &loop;
        condition_.notify();
    }
    loop.loop();
    loop_ = NULL;
}

