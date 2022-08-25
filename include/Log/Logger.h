//
// Created by Svain on 2022/8/6.
//

#ifndef FASTWEBSERVER_LOGGER_H
#define FASTWEBSERVER_LOGGER_H

#include <pthread.h>
#include <stdio.h>
#include <string>

#include "Logging.h"

class Logger {
public:
    Logger(const char *log_name, int num_lines0;
    ~Logger();
    LogStream &log_stream();
    static void set_log_name(string log_name);
    static string get_log_name();

private:
    class Implement {
    public:
        Implement(const char* log_name, int num_lines);
        void format_time();

        LogStream log_stream_;
        int num_lines_;
        string log_name_;

    };
    Implement implement_;
    static string log_name_;

};

#define LOG Logger(__FILE__, __LINE__).stream()

#endif //FASTWEBSERVER_LOGGER_H
