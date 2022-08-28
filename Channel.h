//
// Created by Svain on 2022/8/9.
//

#ifndef FASTWEBSERVER_CHANNEL_H
#define FASTWEBSERVER_CHANNEL_H

#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <queue>

#include "Timer.h"
#include "EventLoop.h"
#include "HttpData.h"
#include "Util.h"
#include "Epoll.h"

using namespace std;

class EventLoop;
class HttpData;

class Channel {
public:
    typedef function<void()> CallBack;
    Channel(EventLoop *event_loop);
    Channel(EventLoop *event_loop, int fd);
    ~Channel();
    int get_fd();
    void set_fd(int fd);
    void set_holder(shared_ptr<HttpData> holder);
    shared_ptr<HttpData> get_holder();
    void set_read_handler(CallBack &&read_handler);
    void set_write_handler(CallBack &&write_handler);
    void set_connect_handler(CallBack &&connect_handler);
    void set_error_handler(CallBack &&error_handler);
    void handle_events();
    void handle_read();
    void handle_write();
    void handle_connect();
    void set_events(__uint32_t events);
    void set_revents(__uint32_t revents);
    bool equal_and_update_last_events();
    __uint32_t& get_events();
    __uint32_t get_last_events();


private:
    typedef function<void()> CallBack;
    EventLoop* event_loop_;
    int fd_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t last_events_;

    weak_ptr<HttpData> holder_;

    int parse_URI();
    int parse_Headers();
    int analyze_request();
    CallBack read_handler_;
    CallBack write_handler_;
    CallBack connect_handler_;
    CallBack error_handler_;

};

//typedef shared_ptr<Channel> channel_sptr;

#endif //FASTWEBSERVER_CHANNEL_H
