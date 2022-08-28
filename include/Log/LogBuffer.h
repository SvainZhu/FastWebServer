//
// Created by Svain on 2022/8/1.
//

#ifndef FASTWEBSERVER_LOGBUFFER_H
#define FASTWEBSERVER_LOGBUFFER_H

#include <string>
#include <assert.h>
#include "../noncopyable.h"
#include "Logging.h"

using namespace std;

class Logging;

template <int SIZE>
class LogBuffer : noncopyable {
public:
    LogBuffer() : cur_(data_) {}
    ~LogBuffer() {}

    const char* get_data() const {
        return data_;
    }

    const int get_length() const {
        return static_cast<int>(cur_ - data_);
    }

    char* get_current() {
        return cur_;
    }

    int get_available_size() {
        return static_cast<int>(end() - cur_);
    }

    void append(const char* buffer, size_t len) {
        if (get_available_size() > static_cast<int>(len)) {
            memcpy(cur_, buffer, len);
            cur_ += len;
        }
    }
    void add(size_t len) {
        cur_ += len;
    }

    void set_zeros() {
        memset(data_, 0, sizeof data_);
    }

    void reset() {
        set_zeros();
        cur_ = data_;
    }

private:
    const char* end() {return data_ + sizeof data_; }
    char data_(SIZE);
    char* cur_;
};


#endif //FASTWEBSERVER_LOGBUFFER_H
