//
// Created by Svain on 2022/8/26.
//

#include "Epoll.h"


const int MAX_EVENTS = 4096;
const int EPOLL_WAIT_TIME = 10000;

Epoll::Epoll() : epoll_fd_(epoll_create1(EPOLL_CLOEXEC)), events_(MAX_EVENTS) {}

Epoll::~Epoll() {}

// add the descriptor into the epoll
void Epoll::add_fd_in_epoll(channel_sptr request, int timeout) {
    int fd = request->get_fd();
    if (timeout > 0) {
        add_timer(request, timeout);
        fd_to_http_[fd] = request->get_holder();
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->get_events();
    request->equal_and_update_last_events();

    fd_to_channel_[fd] = request;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("add fd in epoll error!!!");
        fd_to_channel_[fd].reset();
    }
}

// modify the descriptor in the epoll
void Epoll::modify_fd_in_epoll(channel_sptr request, int timeout) {
    if (timeout > 0) add_timer(request, timeout);
    int fd = request->get_fd();
    if (!request->equal_and_update_last_events()) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = request->get_events();
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            perror("modify fd in epoll error!!!");
            fd_to_channel_[fd].reset();
        }
    }
}

// delete the descriptor from the epoll
void Epoll::delete_fd_in_epoll(channel_sptr request) {
    int fd = request->get_fd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->get_last_events();
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &event) < 0) perror("delete fd in epoll error!!!");
    fd_to_channel_[fd].reset();
    fd_to_http_[fd].reset();

}

// return the active events
vector<channel_sptr> Epoll::poll() {
    while (true) {
        int event_count = epoll_wait(epoll_fd_, &*events_.begin(), events_.size(), EPOLL_WAIT_TIME);
        if (event_count < 0) perror("epoll wait error!!!");
        vector<channel_sptr> request_events = get_events_request(event_count);
        if (request_events() > 0) return request_events;
    }
}

int Epoll::get_epoll_fd() {
    return epoll_fd_;
}

void Epoll::handle_expired() {
    timer_pqueue_.handle_expired_event();
}

vector<channel_sptr> Epoll::get_events_request(int num_events) {
    vector<channel_sptr> request_events;
    for (int i = 0; i < num_events; i++) {
        int fd = events_[i].data.fd;
        channel_sptr current_request = fd_to_channel_[fd];
        if (current_request) {
            current_request->set_revents(events_[i]->events);
            current_request->set_events(0);
            request_events.push_back(current_request);
        }
        else {
            LOG << "Current request pointer is invalid!!!";
        }
    }
    return request_events;
}

void Epoll::add_timer(channel_sptr request_data, int timeout) {
    shared_ptr<HttpData> Http_data = request_data->get_holder();
    if (Http_data) timer_pqueue_.add_timer(Http_data, timeout);
    else LOG << "Add timer is failed!!!"
}