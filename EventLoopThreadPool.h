//
// Created by Svain on 2022/8/15.
//

#ifndef FASTWEBSERVER_EVENTLOOPTHREADPOOL_H
#define FASTWEBSERVER_EVENTLOOPTHREADPOOL_H

#include <memory>
#include <vector>

#include "EventLoopThread.h"
#include "include/Log/Logger.h"
#include "include/noncopyable.h"

using namespace std;

class EventLoopThreadPool : noncopyable {
public:
    EventLoopThreadPool(EventLoop* loop, int num_threads);

    ~EventLoopThreadPool() { LOG <<  "~EventLoopThreadPool()"; }
    void thread_pool_create();
    EventLoop* get_next_loop();

private:
    EventLoop* loop_;
    bool started_;
    int num_threads_;
    int next_;
    vector<shared_ptr<EventLoopThread>> threads_;
    vector<EventLoop*> loops_;
};


#endif //FASTWEBSERVER_EVENTLOOPTHREADPOOL_H
