//
// Created by Svain on 2022/7/13.
//

#include "Condition.h"

void Condition::wait() {
    pthread_cond_wait(&cond, mutex.get());
}

void Condition::notify() {
    pthread_cond_signal(&cond);
}

void Condition::notifyAll() {
    pthread_cond_broadcast(&cond);
}

bool Condition::wait_for_seconds(int seconds) {
    struct timespec abs_time;
    clock_gettime(CLOCK_REALTIME, &abs_time);
    abs_time.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abs_time);

}