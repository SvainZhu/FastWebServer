//
// Created by Svain on 2022/8/20.
//

#include "Server.h"

Server::Server(EventLoop *event_loop, int num_threads, int port) : event_loop_(event_loop), num_threads_(num_threads),
            event_loop_thread_pool_(new EventLoopThreadPool(event_loop_, num_threads)), started_(false),
            accept_channel_(new Channel(event_loop)), port_(port), listen_fd_(set_socket_bind_listen(port)){
    accept_channel_->set_fd(listen_fd_);
    handle_for_signal_pipe();
    if (set_socket_nonblock(listen_fd_) < 0) {
        perror("Set socket with non block failed!!!");
        abort();
    }
}

void Server::start() {
    event_loop_thread_pool_->thread_pool_create();
    accept_channel_->set_events(EPOLLIN | EPOLLET);
    accept_channel_->set_read_handler(bind(&Server::handle_new_connection, this));
    accept_channel_->set_connect_handler(bind(&Server::handle_cur_connection, this));
    event_loop_->add_to_poller(accept_channel_, 0);
    started_ = true;
}

EventLoop* Server::get_event_loop() const {
    return event_loop_;
}

void Server::handle_new_connection() {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof(client_addr);
    int accept_fd = accept(listen_fd_, (struct sockaddr *)&client_addr, &client_addr_len);
    while (accept_fd > 0) {
        EventLoop *event_loop = event_loop_thread_pool_->get_next_loop();
        LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);

        int opt_val = 0;
        socklen_t opt_val_len = 4;
        getsockopt(accept_fd, SOL_SOCKET, SO_KEEPALIVE, &opt_val, &opt_val_len);

        // limit the numbers of connection under the max connection numbers
        if (accept_fd >= MAXFDS_) {
            close(accept_fd);
            continue;
        }
        // set non-blocking mode
        if (set_socket_nonblock(accept_fd) < 0) {
            LOG << "Set non block failed!!!";
            return;
        }

        set_socket_nodelay(accept_fd);
//        set_socket_nolinger(accept_fd);
        shared_ptr<HttpData> request_info(new HttpData(event_loop, accept_fd));
        request_info->get_channel()->set_holder(request_info);
        event_loop->queue_in_loop(bind(&HttpData::new_event, request_info));
    }
    accept_channel_->set_events(EPOLLIN | EPOLLET);
}

void Server::handle_cur_connection() {
    event_loop_->update_poller(accept_channel_);
}