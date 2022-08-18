//
// Created by Svain on 2022/7/9.
//

#include "Thread.h"
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include "CurrentThread.h"

using namespace std;
using namespace CurrentThread;

pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void CurrentThread::cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

// save the thread name, id, etc data.

void ThreadData::runInThread() {
    *tid_ = CurrentThread::tid();
    tid_ = nullptr;
    latch_->countDown();
    latch_ = nullptr;

    CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
    prctl(PR_SET_NAME, CurrentThread::t_threadName);

    func_();
    CurrentThread::t_threadName = "finished";
}

void* startThread(void* obj) {
    ThreadData* thread_data = static_cast<ThreadData*>(obj);
    thread_data->runInThread();
    delete thread_data;
    return nullptr;
}

void Thread::set_default_name() {
    if (name_.empty()) {
        char buffer[32];
        snprintf(buffer, sizeof buffer, "Thread");
        name_ = buffer;
    }
}

Thread::Thread(const ThreadFunction& function, const string &name) :
    started_(false), joined_(false), pid_(0), tid_(0), function_(function), name_(name), latch_(1){
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pid_);
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;
    ThreadData* thread_data = new ThreadData(function_, name_, &tid_, &latch_);
    if (pthread_create(&pid_, nullptr, &startThread, thread_data)) {
        started_ = false;
        delete thread_data;
    }
    else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pid_, nullptr);
}

