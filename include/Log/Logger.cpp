//
// Created by Svain on 2022/8/6.
//

#include "Logger.h"


static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static Logging* logger_;

string Logger::log_name_ = "./FastWebServer.log";

void once_init() {
    logger_ = new Logging(Logger::get_log_name());
    logger_->start();
}

void output(const char* message, int len) {
    pthread_once(&once_control_, once_init);
    logger_->append(message, len);
}

Logger::LogImplement::LogImplement(const char *log_name, int num_lines)
    : log_stream_(), num_lines_(num_lines), log_name_(log_name) {
    format_time();
}

void Logger::LogImplement::format_time() {
    struct timeval tv;
    time_t time;
    char str_time[26] = {0};
    gettimeofday (&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_time, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    log_stream_ << str_time;
}

Logger::Logger(const char *log_name, int num_lines) : log_implement_(log_name, num_lines) {}
Logger::~Logger() {
    log_implement_.log_stream_ << " -- " << log_implement_.log_name_ << ':' << log_implement_.num_lines_ << '\n';
    const LogStream::LogBuffer& buffer(log_stream().get_buffer());
    output(buffer.get_data(), buffer.get_length());
}

LogStream& Logger::log_stream() {
    return log_implement_.log_stream_;
}