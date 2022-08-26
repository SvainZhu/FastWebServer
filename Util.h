//
// Created by Svain on 2022/8/6.
//

#ifndef FASTWEBSERVER_UTIL_H
#define FASTWEBSERVER_UTIL_H

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

const int MAX_BUFFER_SIZE = 4096;

// read function series for the buffer with size n
ssize_t readn(int fd, void *buffer, size_t n);
ssize_t readn(int fd, std::string &buffer_in, bool &zero);
ssize_t readn(int fd, std::string &buffer_in);

// write function series for buffer with size n
ssize_t writen(int fd, void *buffer, size_t n);
ssize_t writen(int fd, std::string &buffer);

// handle for the signal pipe
void handle_for_signal_pipe();

// set socket series
int set_socket_nonblock(int fd);
void set_socket_nodelay(int fd);
void set_socket_nolinger(int fd);
void shutdown_writer(int fd);
int set_socket_bind_listen(int port);

#endif //FASTWEBSERVER_UTIL_H
