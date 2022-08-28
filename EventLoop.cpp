//
// Created by Svain on 2022/8/9.
//

#include "EventLoop.h"

__thread EventLoop* event_loop_in_this_thread = 0;

int create_event_fd() {
    int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (event_fd < 0) {
        LOG << "Event fd failed!!!";
        abort();
    }
    return event_fd;
}

EventLoop::EventLoop() : looping_(false), poller_(new Epoll()), wakeup_fd_(create_event_fd()),
                        quit_(false), event_handling_(false), calling_pending_functors_(false),
                        thread_id_(CurrentThread::tid()), wakeup_channel_sptr_(new Channel(this, wakeup_fd_)){
    if (event_loop_in_this_thread) {

    }
}