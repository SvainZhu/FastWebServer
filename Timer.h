//
// Created by Svain on 2022/8/9.
//

#ifndef FASTWEBSERVER_TIMER_H
#define FASTWEBSERVER_TIMER_H

#include <unistd.h>
#include <deque>
#include <queue>
#include <memory>

#include "include/noncopyable.h"
#include "include/MutexLock.h"
#include "HttpData.h"

class HttpData;

using namespace std;

class Timer {
public:
    Timer(shared_ptr<HttpData> request_data, int timeout);
    ~Timer();
    Timer(Timer &timer);
    bool is_valid();
    bool is_deleted();
    void set_deleted();
    void update(int timeout);
    void clear_request();
    size_t get_expired_time() const;

private:
    bool delete_;
    size_t expired_time_;
    shared_ptr<HttpData> Http_data_sptr;
};

class TimerPQueue {
public:
    TimerPQueue() {};
    ~TimerPQueue() {};
    void add_timer(shared_ptr<HttpData> Http_data_sptr, int timeout);
    void handle_expired_event();
    
private:
    typedef shared_ptr<Timer> timer_sptr;
    struct TimerComp {
        bool operator()(timer_sptr &timer1, timer_sptr &timer2) {
            return timer1->get_expired_time() > timer2->get_expired_time();
        }
    };
    priority_queue<timer_sptr, deque<timer_sptr>, TimerComp> timer_pqueue;
};

#endif //FASTWEBSERVER_TIMER_H
