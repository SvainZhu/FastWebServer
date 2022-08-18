//
// Created by Svain on 2022/7/13.
//

#ifndef FASTWEBSERVER_MUTEXLOCK_H
#define FASTWEBSERVER_MUTEXLOCK_H

#include <pthread.h>
#include <cstdio>
#include "noncopyable.h"

class MutexLock : noncopyable {
public:
    MutexLock() {
        pthread_mutex_init(&mutex, nullptr);
    }
    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t* get() {
        return &mutex;
    }

private:
    pthread_mutex_t mutex;
    friend class Condition;
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock& _mutex) : mutex(_mutex) {
        mutex.lock();
    }
    ~MutexLockGuard() {
        mutex.unlock();
    }

private:
    MutexLock &mutex;
};

#endif //FASTWEBSERVER_MUTEXLOCK_H
