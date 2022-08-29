//
// Created by Svain on 2022/8/9.
//

#ifndef FASTWEBSERVER_HTTPSTREAM_H
#define FASTWEBSERVER_HTTPSTREAM_H
#include <unistd.h>
#include <sys/epoch.h>
#include <functional>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include <time.h>

#include "Timer.h"
#include "EventLoop.h"
#include "Channel.h"

class Timer;
class EventLoop;
class Channel;

// http process state
enum ProcessState {
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH,
};

// URT process state
enum URIState {
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS,
};

// header process state
enum HeaderState {
    PARSE_HEADER_SUCCESS,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR,
};

// request analysis state
enum AnalysisState {
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR,
};

// tcp connection state
enum ConnectionState {
    H_CONNECTED = 0,
    H_DISCONNECTING,
    H_DISCONNECTED
};

// header read state
enum ParseState {
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

// Http method
enum HttpMethod {
    METHOD_POST = 1,
    METHOD_GET,
    METHOD_HEAD,
};

// Http version
enum HttpVersion {
    HTTP_10 = 1,
    HTTP_11,
};

class MimeType {
public:
    static string get_mime(const string &suffix);

private:
    MimeType();
    ~MimeType();
    MimeType(const MimeType &mime);
    static void init();
    static unordered_map<string, string> mime;
    static pthread_once_t once_control;
};

class HttpData : public enable_shared_from_this<HttpData> {
public:
    HttpData(EventLoop* event_loop, int connection_fd);
    ~HttpData();
    void reset();
    void link_timer(shared_ptr<Timer> timer);
    void seperate_timer();
    shared_ptr<Channel> get_channel();
    EventLoop* get_event_loop();
    void handle_close();
    void new_event();

private:
    EventLoop event_loop_;
    shared_ptr<Channel> channel_;
    int fd_;
    string buffer_in_;
    string buffer_out_;
    bool is_error_;
    string file_name_;
    string path_;
    int read_cur_;
    bool keep_alive_;
    map<string, string> headers_;
    weak_ptr<Timer> timer_;

    ConnectionState connection_state_;
    HttpMethod Http_method_;
    HttpVersion Http_version_;
    ProcessState process_state_;
    // header read state
    ParseState parse_state_;


    void handle_read();
    void handle_write();
    void handle_connect();
    void handle_error(int fd, int error_code, string short_message);
    URIState parse_URI();
    HeaderState parse_Headers();
    AnalysisState analyze_request();

};


#endif //FASTWEBSERVER_HTTPSTREAM_H
