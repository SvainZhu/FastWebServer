//
// Created by Svain on 2022/8/1.
//

#ifndef FASTWEBSERVER_LOGGING_H
#define FASTWEBSERVER_LOGGING_H
#include <functional>
#include <string>
#include <vector>
#include <ctime>

#include "../noncopyable.h"
#include "../MutexLock.h"
#include "../Thread.h"
#include "../CountDownLatch.h"
#include "../Condition.h"
#include "LogBuffer.h"
#include "LogUtil.h"

using namespace std;

const int small_buffer = 4096;
const int big_buffer = small_buffer * 1000;

class LogStream : noncopyable {
public:
    typedef LogBuffer<small_buffer> LogBuffer;

    LogStream& operator<<(bool v) {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float v);
    LogStream& operator<<(double);
    LogStream& operator<<(long double);
    LogStream& operator<<(char v);
    LogStream& operator<<(const char* str);
    LogStream& operator<<(const unsigned char* str);
    LogStream& operator<<(const string& v);

    const LogBuffer& get_buffer() const {return buffer_};
    void append(const char* log_line, int len);
    void reset_buffer();

private:
    template<typename T>
    void fromat_integer(T);

    LogBuffer buffer_;
    static const int max_digit_size = 32;
};

class Logging : noncopyable {
public:
    Logging(const string log_name, int flush_interval = 2);
    ~Logging() {
        if (is_running_) stop();
    }

    void append(const char *log_line, const int len);
    void start();
    void stop();

private:
    void thread_func();
    typedef LogBuffer<small_buffer> LogBuffer;
    typedef vector<shared_ptr<LogBuffer>> BufferList;
    typedef shared_ptr<LogBuffer> BufferPtr;
    string log_name_;
    bool is_running_;
    const int flush_interval_ = 10;
    MutexLock mutex_lock_;
    Condition cond_;
    Thread thread_;
    BufferPtr cur_buffer_;
    BufferPtr next_buffer_;
    BufferList buffer_list_;
    CountDownLatch latch_;

};



#endif //FASTWEBSERVER_LOGGING_H
