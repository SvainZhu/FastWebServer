//
// Created by Svain on 2022/8/1.
//

#ifndef FASTWEBSERVER_LOGBUFFER_H
#define FASTWEBSERVER_LOGBUFFER_H

#include <string>
#include <assert.h>
#include "../noncopyable.h"

using namespace std;

template <int SIZE>
class LogBuffer : noncopyable {
public:
    LogBuffer() : cur_(data_) {}
    ~LogBuffer();

    const char* get_data();
    const int get_length();
    const char* get_current();
    int get_available_size();
    void append(const char* buffer, size_t len);
    void add(size_t len);
    void reset();
    void set_zeros();


private:
    const char* end() {return data_ + sizeof data_; }
    char data_(SIZE);
    char* cur_;
};


#endif //FASTWEBSERVER_LOGBUFFER_H
