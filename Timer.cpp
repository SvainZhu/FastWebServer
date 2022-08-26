//
// Created by Svain on 2022/8/9.
//

#include "Timer.h"

Timer::Timer(shared_ptr<HttpData> request_data, int timeout) : delete_(false), Http_data_sptr(request_data) {
    struct timeval now;
    gettimeofday(&now, NULL);

    // get expired time (ms)
    expired_time_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;

}

Timer::~Timer() {
    if (Http_data_sptr) Http_data_sptr->handleClose();
}

Timer::Timer(Timer &timer) : Http_data_sptr(timer.Http_data_sptr), expired_time_(0) {}

void Timer::set_deleted() {
    delete_ = true;
}

bool Timer::is_valid() {
    struct timeval now;
    gettimeofday(&now, NULL);

    // invert the current time into time with ms
    size_t temp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
    if (temp < expired_time_) return true;
    else {
        this->set_deleted();
        return false;
    }
}

bool Timer::is_deleted() {
    return delete_;
}

void Timer::update(int timeout) {
    struct timeval now;
    gettimeofday(&now, NULL);

    // update the expired time (ms)
    expired_time_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
}

size_t Timer::get_expired_time() const {
    return expired_time_;
}

void Timer::clear_request() {
    Http_data_sptr.reset();
    this->set_deleted();
}

void TimerPQueue::add_timer(shared_ptr<HttpData> Http_data_sptr, int timeout) {
    timer_sptr timer(new Timer(Http_data_sptr, timeout));
    timer_pqueue.push(timer);
    Http_data_sptr->link_timer(timer);
}


/* The priority queue cannot to visit in random. Besides, it needs to update entire heap structure after delete a node.
 * Therefore, the node to be deleted will be delay to delete when the node is timeout or all higher priority nodes than
 * it were deleted in order to save the time and improve performance. Delaying delete operator in max TIMEOUT time also
 * will provide a cache time to keep a request data node for escaping the re-new and re-delete of a new node when a new
 * request reaches the server.
 */

void TimerPQueue::handle_expired_event() {
    while (!timer_pqueue.empty()) {
        timer_sptr cur_timer = timer_pqueue.top();
        if (cur_timer->is_deleted() || !cur_timer->is_valid()) timer_pqueue.pop();
        else break;
    }
}