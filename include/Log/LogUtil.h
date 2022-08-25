//
// Created by Svain on 2022/8/1.
//

#ifndef FASTWEBSERVER_LOGUTIL_H
#define FASTWEBSERVER_LOGUTIL_H
#include <string>
#include "../noncopyable.h"
#include "../MutexLock.h"

using namespace std;

class AppendUtil : noncopyable {
public:
    explicit AppendUtil(string file_name);
    ~AppendUtil();
    void append(const char *string_line, const size_t len);
    void flush();
private:
    size_t write(const char *string_line, const size_t len);
    FILE *fptr_;
    char buffer_[64 * 1024];
};

class AppendLog : noncopyable {
public:
    AppendLog(const string &log_name, int num_append = 1000);
    ~AppendLog();

    void append(const char *log_line, const int len);
    void flush();
    bool roll_file();

private:
    void append_unlocked(const char *log_line, const int len);
    const string log_name_;
    const int num_append_;
    int count_;
    unique_ptr<MutexLock> mutex_;
    unique_ptr<AppendUtil> file_;

};


#endif //FASTWEBSERVER_LOGUTIL_H
