//
// Created by Svain on 2022/8/20.
//

#ifndef FASTWEBSERVER_SERVER_H
#define FASTWEBSERVER_SERVER_H

#include <memory>
#include <functional>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Util.h"
#include "include/Log/Logger.h"
#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

class Server {
public:
    Server(EventLoop* event_loop, int num_threads, int port);
    ~Server() {}
    EventLoop *get_event_loop() const;
    void start();
    void handle_new_connection();
    void handle_cur_connection();

private:
    EventLoop* event_loop_;
    int num_threads_;
    unique_ptr<EventLoopThreadPool> event_loop_thread_pool_;
    bool started_;
    shared_ptr<Channel> accept_channel_;
    int port_;
    int listen_fd_;
    static const int MAXFDS_ = 100000;
};

#endif //FASTWEBSERVER_SERVER_H
