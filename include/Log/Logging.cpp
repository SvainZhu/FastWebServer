//
// Created by Svain on 2022/8/1.
//

#include "Logging.h"


// convert digit value into digits string
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
template <typename T>
size_t convert(char* buffer, T value) {
    T num = value;
    char* cur = buffer;
    int digit = 0;
    do {
        digit = static_cast<int>(num % 10);
        num /= 10;
        *cur++ = zero[digit];
    } while (num != 0);

    if (value < 0) *cur++ = '-';
    *cur = '\0';
    reverse(buffer, cur);
    return cur - buffer;
}

template class LogBuffer<small_buffer>;
template class LogBuffer<big_buffer>;
template <typename T>
void LogStream::fromat_integer(T value) {
    if (buffer_.get_available_size() >= max_digit_size) {
        size_t length = convert(buffer_.get_current(), value);
        buffer_.add(length);
    }
}

LogStream& LogStream::operator<<(short value) {
    *this << static_cast<int>(value);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short value) {
    *this << static_cast<unsigned int>(value);
    return *this;
}

LogStream& LogStream::operator<<(int value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(long value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(long long value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long value) {
    fromat_integer(value);
    return *this;
}

LogStream& LogStream::operator<<(double value) {
    if (buffer_.get_available_size() >= max_digit_size) {
        int length = snprintf(buffer_.get_current(), max_digit_size, "%.12g", value);
        buffer_.add(length);
    }
    return *this;
}

LogStream& LogStream::operator<<(float value) {
    *this << static_cast<double>(value);
    return *this;
}

LogStream& LogStream::operator<<(long double value) {
    if (buffer_.get_available_size() >= max_digit_size) {
        int length = snprintf(buffer_.get_current(), max_digit_size, "%.12lg", value);
        buffer_.add(length);
    }
    return *this;
}

LogStream& LogStream::operator<<(char v) {
    buffer_.append(&v, 1);
    return *this;
}

LogStream& LogStream::operator<<(const char* str) {
    if (str) buffer_.append(str, strlen(str));
    else buffer_.append("_null_", 6);
    return *this;
}

LogStream& LogStream::operator<<(const unsigned char* str) {
    return operator<<(reinterpret_cast<const char*>(str));
}

LogStream& LogStream::operator<<(const string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
}

void LogStream::append(const char* log_line, int len) {
    buffer_.append(log_line, len);
}

void LogStream::reset_buffer() {
    buffer_.reset();
}

Logging::Logging(const string log_name, int flush_interval) : flush_interval_(flush_interval), is_running_(false),
    log_name_(log_name), thread_(bind(&Logging::thread_func, this), "Logging"), mutex_lock_(), cond_(mutex_lock_),
    cur_buffer_(new LogBuffer), next_buffer_(new LogBuffer), buffer_list_(), latch_(1){
    assert(log_name_.size() > 1);
    cur_buffer_->set_zeros();
    next_buffer_->set_zeros();
    std::reverse(buffer_list_.begin(), buffer_list_.end());
}

void Logging::append(const char *log_line, const int len) {
    MutexLockGuard lock(mutex_lock_);
    if (cur_buffer_->get_available_size() > len) cur_buffer_->append(log_line, len);
    else {
        buffer_list_.push_back(cur_buffer_);
        cur_buffer_.reset();
        if (next_buffer_) cur_buffer_ = std::move(next_buffer_);
        else cur_buffer_.reset(new LogBuffer);
        cur_buffer_->append(log_line, len);
        cond_.notify();
    }
}

void Logging::start() {
    is_running_ = true;
    thread_.start();
    latch_.wait();
}

void Logging::stop() {
    is_running_ = false;
    cond_.notify();
    thread_.join();
}

void Logging::thread_func() {
    assert(is_running_ == true);
    latch_.count_down();
    AppendLog logging_output(log_name_);
    BufferPtr log_buffer1(new LogBuffer), log_buffer2(new LogBuffer);
    BufferList  buffer_list_output;
    log_buffer1->set_zeros();
    log_buffer2->set_zeros();
    buffer_list_output.reserve(16);
    while (is_running_) {
        assert(log_buffer1 && log_buffer1->get_length() == 0);
        assert(log_buffer2 && log_buffer2->get_length() == 0);
        assert(buffer_list_output.empty());

        {
            MutexLockGuard lock(mutex_lock_);
            if (buffer_list_.empty()) cond_.wait_for_seconds(flush_interval_);
            buffer_list_.push_back(cur_buffer_);
            cur_buffer_.reset();

            cur_buffer_ = std::move(log_buffer1);
            buffer_list_output.swap(buffer_list_);
            if (!next_buffer_) next_buffer_ = std::move(log_buffer2);
        }


        assert(!buffer_list_output.empty());
        if (buffer_list_output.size() > 25) {
//            char buffer[256];
//            snprintf(buffer, sizeof buffer, "Drop log messages at %s, %zd larger buffers\n",
//                     Timestamp::now().toFormattedString().c_str(), buffer_list_output.size()-2);
//            fputs(buffer, stderr);
//            logging_output.append(buffer, static_cast<int>(strlen(buffer)));
            buffer_list_output.erase(buffer_list_output.begin() + 2, buffer_list_output.end());
        }

        for (const auto &buffer_output : buffer_list_output) {
            logging_output.append(buffer_output->get_data(), buffer_output->get_length());
        }

        // drop non-zero buffers
        if (buffer_list_output.size() > 2) {
            buffer_list_output.resize(2);
        }

        if (!log_buffer1) {
            assert(!buffer_list_output.empty());
            log_buffer1 = buffer_list_output.back();
            buffer_list_output.pop_back();
            log_buffer1.reset();
        }

        if (!log_buffer2) {
            assert(!buffer_list_output.empty());
            log_buffer2 = buffer_list_output.back();
            buffer_list_output.pop_back();
            log_buffer2.reset();
        }

        buffer_list_output.clear();
        logging_output.flush();
    }
    logging_output.flush();

}