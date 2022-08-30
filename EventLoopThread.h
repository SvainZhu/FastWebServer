//
// Created by Svain on 2022/8/15.
//

#ifndef FASTWEBSERVER_EVENTLOOPTHREAD_H
#define FASTWEBSERVER_EVENTLOOPTHREAD_H

#include <functional>

#include "EventLoop.h"
#include "include/noncopyable.h"
#include "include/Condition.h"
#include "include/MutexLock.h"
#include "include/Thread.h"


class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop* start_loop();

private:
    void thread_func();
    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_lock_;
    bool exiting_;
    Condition condition_;

};


#endif //FASTWEBSERVER_EVENTLOOPTHREAD_H
