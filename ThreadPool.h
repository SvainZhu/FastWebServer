//
// Created by Svain on 2022/8/30.
//

#ifndef FASTWEBSERVER_THREADPOOL_H
#define FASTWEBSERVER_THREADPOOL_H

#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>

#include "Channel.h"

using namespace std;

const int MAX_THREADS = 1024;
const int MAX_QUEUE = 65535;
const unordered_map<string, int> THREAD_STATE{{"THREADPOOL_INVALID", -1}, {"THREADPOOL_LOCK_FAILURE", -2},
                                              {"THREADPOOL_QUEUE_FULL", -3}, {"THREADPOOL_SHUTDOWN", -4},
                                              {"THREADPOOL_THREAD_FAILURE", -5}, {"THREADPOOL_GRACEFUL", 1}};
typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown = 2
} ShutdownOption;

struct ThreadPoolTask {
    function<void(shared_ptr<void>)> func;
    shared_ptr<void> args;
};

class ThreadPool {
public:
    static int thread_pool_create(int num_threads,int queue_size);
    static int thread_pool_add(shared_ptr<void> args, function<void(shared_ptr<void>)> func);
    static int thread_pool_destory(ShutdownOption shutdown_option = graceful_shutdown);
    static int thread_pool_free();
    static void *thread_pool_thread(void *args);

private:
    static pthread_mutex_t mutex_lock_;
    static pthread_cond_t notify_;
    vector<pthread_t> threads_;
    vector<ThreadPoolTask> queue_;
    static int thread_count_;
    static int queue_size_;
    static int head_;
    static int tail_;
    static int count_;
    static int shutdown_;
    static int started_;

};


#endif //FASTWEBSERVER_THREADPOOL_H
