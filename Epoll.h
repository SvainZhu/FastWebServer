//
// Created by Svain on 2022/8/26.
//

#ifndef FASTWEBSERVER_EPOLL_H
#define FASTWEBSERVER_EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"

using namespace std;

typedef shared_ptr<Channel> channel_sptr;

class Epoll {
public:
    Epoll();
    ~Epoll();
    void epoll_add(channel_sptr request, int timeout);
    void epoll_delete(channel_sptr request);
    void epoll_modify(channel_sptr request, int timeout);
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
