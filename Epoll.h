//
// Created by Svain on 2022/8/26.
//

#ifndef FASTWEBSERVER_EPOLL_H
#define FASTWEBSERVER_EPOLL_H

#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string>
#include <deque>
#include <queue>
#include <arpa/inet.h>
#include <iostream>

#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"
#include "include/Log/Logger.h"

using namespace std;

typedef shared_ptr<Channel> channel_sptr;

class Epoll {
public:
    Epoll();
    ~Epoll();
    void add_fd_in_epoll(channel_sptr request, int timeout);
    void delete_fd_in_epoll(channel_sptr request);
    void modify_fd_in_epoll(channel_sptr request, int timeout);
    vector<channel_sptr> poll();
    vector<channel_sptr> get_events_request(int num_events);
    int get_epoll_fd();
    void handle_expired();
    void add_timer(channel_sptr request_data, int timeout);

private:
    static const int MAX_FDS = 100000;
    int epoll_fd_;
    vector<epoll_event> events_;
    channel_sptr fd_to_channel_[MAX_FDS];
    shared_ptr<HttpData> fd_to_http_[MAX_FDS];
    TimerPQueue timer_pqueue_;

};


#endif //FASTWEBSERVER_EPOLL_H
