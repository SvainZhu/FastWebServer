//
// Created by Svain on 2022/8/6.
//

#include "Util.h"

// read function series for the buffer with size n
ssize_t readn(int fd, void* buffer, size_t n) {
    size_t n_left = n;
    ssize_t n_read = 0;
    ssize_t sum_read = 0;
    char* buff_ptr = (char *)buffer;
    while (n_left > 0) {
        n_read = read(fd, buff_ptr, n_left);
        if (n_read < 0) {
            if (errno == EINTR) n_read = 0;
            else if (errno == EAGAIN) return sum_read;
            else return -1;
        }
        else if (n_read = 0) break;
        sum_read += n_read;
        n_left -= n_read;
        buff_ptr += n_read;
    }
    return sum_read;
}

ssize_t readn(int fd, std::string &buffer_in, bool &zero) {
    ssize_t n_read = 0;
    ssize_t sum_read = 0;
    while (true) {
        char buffer[MAX_BUFFER_SIZE];
        n_read = read(fd, buffer, MAX_BUFFER_SIZE);
        if (n_read < 0) {
            if (errno == EINTR) continue;
            else if (errno == EAGAIN) return sum_read;
            else return -1;
        }
        else if (n_read == 0) {
            zero = true;
            break;
        }
        sum_read += n_read;
        buffer_in += std::string(buffer, buffer + n_read);
    }
    return sum_read;
}

ssize_t readn(int fd, std::string &buffer_in) {
    ssize_t n_read = 0;
    ssize_t sum_read = 0;
    while (true) {
        char buffer[MAX_BUFFER_SIZE];
        n_read = read(fd, buffer, MAX_BUFFER_SIZE);
        if (n_read < 0) {
            if (errno == EINTR) continue;
            else if (errno == EAGAIN) return sum_read;
            else return -1;
        }
        else if (n_read == 0) break;
        sum_read += n_read;
        buffer_in += std::string(buffer, buffer + n_read);
    }
    return sum_read;
}


// write function series for buffer with size n
ssize_t writen(int fd, void *buffer, size_t n) {
    size_t n_left = n;
    ssize_t n_write = 0;
    ssize_t sum_write = 0;
    char *buffer_ptr = (char *)buffer;
    while (n_left > 0) {
        n_write = write(fd, buffer_ptr, n_left);
        if (n_write < 0) {
            if (errno == EINTR) {
                n_write = 0;
                continue;
            }
            else if (errno == EAGAIN) return sum_write;
            else return -1;
        }
        sum_write += n_write;
        n_left -= n_write;
        buffer_ptr += n_write;
    }
    return sum_write;
}

ssize_t writen(int fd, std::string &buffer) {
    size_t n_left = buffer.size();
    ssize_t n_write = 0;
    ssize_t sum_write = 0;
    const char *buffer_ptr = buffer.c_str();
    while (n_left > 0) {
        n_write = write(fd, buffer_ptr, n_left);
        if (n_write < 0) {
            if (errno == EINTR) {
                n_write = 0;
                continue;
            }
            else if (errno == EAGAIN) break;
            else return -1;
        }
        sum_write += n_write;
        n_left -= n_write;
        buffer_ptr += n_write;
    }
    if (sum_write == static_cast<int>(buffer.size())) buffer.clear();
    else buffer = buffer.substr(sum_write);
    return sum_write;
}

// handle for the signal pipe
void handle_for_signal_pipe() {
    struct sigaction sign_action;
    memset(&sign_action, '\0', sizeof(sign_action));
    sign_action.sa_handler = SIG_IGN;
    sign_action.sa_flags = 0;
    if (sigaction(SIGPIPE, &sign_action, NULL)) return;
}

int set_socket_nonblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) return -1;
    flag |= O_NONBLOCK;
    flag = fcntl(fd, F_SETFL, flag);
    if (flag == -1) return -1;
    return 0;
}

void set_socket_nodelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));
}

void set_socket_nolinger(int fd) {
    struct linger linger_;
    linger_.l_onoff = 1;
    linger_.l_linger = 30;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&linger_, sizeof(linger_));

}

void shutdown_writer(int fd) {
    shutdown(fd, SHUT_WR);
}

int set_socket_bind_listen(int port) {
    assert(port >= 0 && port <= 65535);

    // create the listen socket and return the listen descriptor
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) return -1;

    int opt_val = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
        close(listen_fd);
        return -1;
    }

    // set the IP and port of server, bind them with listen descriptor
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1) {
        close(listen_fd);
        return -1;
    }

    // listen the port, the max length is 2048
    if (listen(listen_fd, 2048) == -1) {
        close(listen_fd);
        return -1;
    }

    // close the invalid listen descriptor
    if (listen_fd == -1) {
        close(listen_fd);
        return -1;
    }
    return listen_fd;

}