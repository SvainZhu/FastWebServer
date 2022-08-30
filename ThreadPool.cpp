//
// Created by Svain on 2022/8/30.
//

#include "ThreadPool.h"

pthread_mutex_t ThreadPool::mutex_lock_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::notify_ = PTHREAD_COND_INITIALIZER;
int ThreadPool::thread_count_ = 0;
int ThreadPool::queue_size_ = 0;
int ThreadPool::head_ = 0;
int ThreadPool::tail_ = 0;
int ThreadPool::count_ = 0;
int ThreadPool::shutdown_ = 0;
int ThreadPool::started_ = 0;

int ThreadPool::thread_pool_create(int num_threads, int queue_size) {
    bool is_error = false;
    do {
        if (num_threads <= 0 || num_threads > MAX_THREADS || queue_size <= 0 || queue_size > MAX_QUEUE) {
            num_threads = 4;
            queue_size = 1024;
        }

        thread_count_ = 0;
        queue_size_ = queue_size;
        head_ = tail_ = count_ = 0;
        shutdown_ = started_ = 0;
        threads_.resize(num_threads);
        queue_.resize(queue_size);

        // start worker threads
        for (int i = 0; i < num_threads; i++) {
            if (pthread_create(&threads_[i], NULL, thread_pool_thread, (void*)(0)) != 0) {
                return -1;
            }
            thread_count_++;
            started_++;
        }
    } while (false);

    if (is_error) {
        return -1;
    }
    return 0;
}

int ThreadPool::thread_pool_add(shared_ptr<void> args, function<void(shared_ptr<void>)> func) {
    int next, error = 0;
    if (pthread_mutex_lock(&mutex_lock_) != 0) return THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
    do {
        next = (tail_ + 1) % queue_size_;
        if (count_ == queue_size_) {
            error = THREAD_STATE["THREADPOOL_QUEUE_FULL"];
            break;
        }
        if (shutdown_) {
            error = THREAD_STATE["THREADPOOL_SHUTDOWN"];
            break;
        }
        queue_[tail_].func = func;
        queue_[tail_].args = args;
        tail_ = next;
        count_++;

        if (pthread_cond_signal(&notify_) != 0) {
            error = THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
            break;
        }

    } while(false);

    if (pthread_mutex_unlock(&mutex_lock_) != 0) error = THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
    return error;
}

int ThreadPool::thread_pool_destory(ShutdownOption shutdown_option) {
    int error = 0;
    if (pthread_mutex_lock(&mutex_lock_) != 0) return THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
    do {
        if (shutdown_) {
            error = THREAD_STATE["THREADPOOL_SHUTDOWN"];
            break;
        }
        shutdown_ = shutdown_option;

        if ((pthread_cond_broadcast(&notify_) != 0) || (pthread_mutex_unlock(&mutex_lock_) != 0)) {
            error = THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
            break;
        }

        for (int i = 0; i < thread_count_; i++) {
            if (pthread_join(threads_[i], NULL) != 0) error = THREAD_STATE["THREADPOOL_LOCK_FAILURE"];
        }
    } while (false);

    if (!error) thread_pool_free();
    return error;
}

int ThreadPool::thread_pool_free() {
    if (started_ > 0) return -1;
    pthread_mutex_lock(&mutex_lock_);
    pthread_mutex_lock(&mutex_lock_);
    pthread_cond_lock(&notify_);
    return 0;
}

void* ThreadPool::thread_pool_thread(void *args) {
    while (true) {
        ThreadPoolTask task;
        pthread_mutex_lock(&mutex_lock_);
        while ((count_ == 0) && (!shutdown_)) pthread_cond_wait(&notify_, &mutex_lock_);
        if ((shutdown_ == immediate_shutdown) || (shutdown_ == graceful_shutdown) && (count_ == 0)) break;
        task.func = queue_[head_].func;
        task.args = queue_[head_].args;
        queue_[head_].func = NULL;
        queue_[head_].args.reset();
        head_ = (head_ + 1) % queue_size_;
        --count_;
        pthread_mutex_unlock(&mutex_lock_);
        (task.func)(task.args);
    }
    --started_;
    pthread_mutex_unlock(&mutex_lock_);
    pthread_exit(NULL);
    return NULL;
}