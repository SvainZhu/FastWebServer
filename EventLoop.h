//
// Created by Svain on 2022/8/9.
//

#ifndef FASTWEBSERVER_EVENTLOOP_H
#define FASTWEBSERVER_EVENTLOOP_H

#include <vector>
#include <functional>
#include <memory>
#include <iostream>

#include "include/Thread.h"
#include "include/CurrentThread.h"
#include "include/Log/Logger.h"
#include "Channel.h"
#include "Util.h"
#include "Epoll.h"

using namespace std;


class EventLoop {
public:
    typedef function<void()> functor;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void run_in_loop(functor&& cb);
    void queue_in_loop(functor&& cb);
    bool is_thread_in_loop() const;
    void assert_thread_in_loop();
    void shutdown(shared_ptr<Channel> channel);
    void remove_from_poller(shared_ptr<Channel> channel);
    void update_poller(shared_ptr<Channel> channel, int timeout = 0);
    void add_poller(shared_ptr<Channel> channel, int timeout = 0);

private:
    bool looping_;
    bool quit_;
    bool event_handling_;
    bool calling_pending_functors_;
    vector<functor> pending_functors_;
    mutable MutexLock mutex_lock_;
    int wakeup_fd_;
    shared_ptr<Epoll> poller_;
    const pid_t thread_id_;
    shared_ptr<Channel> wakeup_channel_sptr_;

    void wakeup();
    void handle_read();
    void handle_connect();
    void do_pending_functors();

};


#endif //FASTWEBSERVER_EVENTLOOP_H
