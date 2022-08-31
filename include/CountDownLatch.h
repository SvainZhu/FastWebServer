//
// Created by Svain on 2022/7/9.
//

#ifndef FASTWEBSERVER_COUNTDOWNLATCH_H
#define FASTWEBSERVER_COUNTDOWNLATCH_H

#include "noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"

class CountDownLatch :noncopyable {
public:
    explicit CountDownLatch(int count);
    void wait();

    void count_down();

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};


#endif //FASTWEBSERVER_COUNTDOWNLATCH_H
