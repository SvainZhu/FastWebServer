//
// Created by Svain on 2022/7/9.
//

#ifndef FASTWEBSERVER_THREAD_H
#define FASTWEBSERVER_THREAD_H

#pragma once
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <string>

#include "noncopyable.h"
#include "CountDownLatch.h"
#include "CurrentThread.h"

using namespace std;
using namespace CurrentThread;

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunction;
    Thread(const ThreadFunction&, const string& name = string());
    ~Thread();

    void start();

    int join();

    bool started() const {return started_;}
    pid_t tid() const {return tid_};
    const string& name() const {return name_;}

private:
    bool started_;
    pid_t tid_;
    string name_;
    ThreadFunction function_;
    CountDownLatch latch_;
};


#endif //FASTWEBSERVER_THREAD_H
