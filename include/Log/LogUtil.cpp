//
// Created by Svain on 2022/8/1.
//

#include "LogUtil.h"

AppendUtil::AppendUtil(string file_name) : fptr_(fopen(file_name.c_str(), "ae")) {
    setbuffer(fptr_, buffer_, sizeof buffer_);
}

AppendUtil::~AppendUtil() { fclose(fptr_); }

void AppendUtil::append(const char *string_line, const size_t len) {
    size_t finished = this->write(string_line, len);
    size_t remained = len - finished;
    while (remained > 0) {
        size_t new_finished = this->write(string_line + finished, remained);
        if (new_finished == 0) {
            if (ferror(fptr_)) fprintf(stderr, "AppendUtil::");
            break;
        }
        finished += new_finished;
        remained = len - finished;
    }
}

void AppendUtil::flush() {
    fflush(fptr_);
}

size_t AppendUtil::write(const char *string_line, const size_t len) {
    return fwrite(string_line, 1, len, fptr_);
}

AppendLog::AppendLog(const string &log_name, int num_append) : log_name_(log_name), num_append_(num_append),
            count_(0), mutex_(new MutexLock){
    file_.reset(new AppendUtil(log_name));
}

void AppendLog::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void AppendLog::append_unlocked(const char *log_line, const int len) {
    file_->append(log_line, len);
    count_++;
    if (count_ >= num_append_) {
        count_ = 0;
        file_->flush();
    }
}

void AppendLog::append(const char *log_line, const int len) {
    MutexLockGuard lock(*mutex_);
    append_unlocked(log_line, len);
}

