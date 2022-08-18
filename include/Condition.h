//
// Created by Svain on 2022/7/13.
//

#ifndef FASTWEBSERVER_CONDITION_H
#define FASTWEBSERVER_CONDITION_H

#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "noncopyable.h"
#include "MutexLock.h"

class Condition : noncopyable {
public:
    explicit Condition(MutexLock& _mutex) : mutex(_mutex) {
        pthread_cond_init(&cond, nullptr);
    }
    ~Condition() {
        pthread_cond_wait(&cond);
    }
    void wait();
    void notify();
    void notifyAll();
    bool wait_for_seconds(int seconds);

private:
    pthread_cond_t cond;
    MutexLock& mutex;
};

#endif //FASTWEBSERVER_CONDITION_H
