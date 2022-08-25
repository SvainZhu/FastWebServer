//
// Created by Svain on 2022/8/1.
//

#include "LogBuffer.h"

const char* LogBuffer::get_data() {
    return data_;
}

const int LogBuffer::get_length() {
    return static_cast<int>(cur_ - data_);
}

const char* LogBuffer::get_current() {
    return cur_
}

int LogBuffer::get_available_size() {
    return static_cast<int>(end() - cur_);
}

void LogBuffer::append(const char* buffer, size_t len) {
    if (get_available_size() > static_cast<int>(len)) {
        memcpy(cur_, buffer, len);
        cur_ += len;
    }
}
void LogBuffer::add(size_t len) {
    cur_ += len;
}

void LogBuffer::set_zeros() {
    memset(data_, 0, sizeof data_);
}

void LogBuffer::reset() {
    set_zeros();
    cur_ = data_;
}
