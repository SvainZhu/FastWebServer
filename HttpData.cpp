//
// Created by Svain on 2022/8/9.
//

#include "HttpData.h"

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
unordered_map<string, string> MimeType::mime;

const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;                  // ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;      // ms

char favicon[555] = {
        '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
        '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
        '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
        '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
        '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
        'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
        'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
        'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
        '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
        '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
        'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
        'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
        'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
        '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
        '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
        '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
        'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
        '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
        '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
        '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
        '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
        '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
        '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
        'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
        '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
        '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
        '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
        'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
        'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
        '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
        '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
        '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
        '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
        '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
        '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
        '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
        '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
        '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
        '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
        'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
        'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
        '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
        '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
        '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
        '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
        'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
        '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
        '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
        '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
        '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
        '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
        '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
        '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
        '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
        '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
        'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
        '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
        '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
        '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
        '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
        'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
        'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};


void MimeType::init() {
    mime[".html"] = "text/html";
    mime[".avi"] = "video/x-msvideo";
    mime[".bmp"] = "image/bmp";
    mime[".c"] = "text/plain";
    mime[".doc"] = "application/msword";
    mime[".gif"] = "image/gif";
    mime[".gz"] = "application/x-gzip";
    mime[".htm"] = "text/html";
    mime[".ico"] = "image/x-icon";
    mime[".jpg"] = "image/jpeg";
    mime[".png"] = "image/png";
    mime[".txt"] = "text/plain";
    mime[".mp3"] = "audio/mp3";
    mime["default"] = "text/html";
}

string MimeType::get_mime(const string &suffix) {
    pthread_once(&once_control, MimeType::init);
    if (mime.find(suffix) == mime.end()) return mime["default"];
    else return mime[suffix];
}

HttpData::HttpData(EventLoop *event_loop, int connection_fd) : event_loop_(event_loop),
    channel_(new Channel(event_loop, connection_fd)), fd_(connection_fd), is_error_(false),
    connection_state_(H_CONNECTED), Http_method_(METHOD_GET), Http_version_(HTTP_11), read_cur_(0),
    process_state_(STATE_PARSE_URI), prase_state_(H_START), keep_alive_(false){
    channel_->set_read_handler(bind(&HttpData::handle_read, this));
    channel_->set_connect_handler(bind(&HttpData::handle_connect, this));
    channel_->set_write_handler(bind(&HttpData::handle_connect, this));
}

void HttpData::reset() {
    buffer_in_.clear();
    file_name_.clear();
    path_.clear();
    read_cur_ = 0;
    process_state_ = STATE_PARSE_URI;
    prase_state_ = H_START;
    headers_.clear();
    keep_alive_ = false;
    if (timer_.lock()) {
        shared_ptr<Timer> timer(timer_.lock());
        timer->clear_request();
        timer_.reset();
    }
}

void HttpData::seperate_timer() {
    if (timer_.lock()) {
        shared_ptr<Timer> timer(timer_.lock());
        timer->clear_request();
        timer_.reset();
    }
}

void HttpData::handle_read() {
    __uint32_t &events_ = channel_->get_events();
    do {
        bool zero = false;
        int num_read = readn(fd_, buffer_in_, zero);
        LOG << "Request: " << buffer_in_;
        if (connection_state_ == H_DISCONNECTED) {
            buffer_in_.clear();
            break;
        }
        if (num_read < 0) {
            perror("1");
            is_error_ = true;
            handle_error(fd_, 400, "Bad Request!!!");
            break;
        }
        else if (zero) {
            // cannot read the data from request. There are many reasons causing it, but here unified processing
            // according to the client end has been closed for simple and convenience.
            is_error_ = true;
            connection_state_ = H_DISCONNECTED;
            if (num_read == 0) break;
        }
        if (process_state_ == STATE_PARSE_URI) {
            URIState flag = this->parse_URI();
            if (flag == PARSE_URI_AGAIN) break;
            else if (flag == PARSE_URI_ERROR) {
                perror("2");
                buffer_in_.clear();
                is_error_ = true;
                LOG << "FD = " << fd_ "," << buffer_in_ << "******";
                handle_error(fd_, 400, "Bad Request!!!");
                beak
            }
            else process_state_ = STATE_PARSE_HEADERS;
        }
        if (process_state_ == STATE_PARSE_HEADERS) {
            HeaderState flag = this->parse_Headers();
            if (flag == PARSE_HEADER_AGAIN) break;
            else if (flag == PARSE_HEADER_ERROR) {
                perror("3");
                is_error_ = true;
                handle_error(fd_, 400, "Bad Request!!!");
                break;
            }
            if (Http_method_ == METHOD_POST) process_state_ = STATE_RECV_BODY;
            else process_state_ = STATE_ANALYSIS;
        }
        if (process_state_ == STATE_RECV_BODY) {
            int content_length = -1;
            if (headers_.find("Content-length") != headers_.end()) content_length = stoi(headers_["Content-length"]);
            else {
                is_error_ = true;
                handle_error(fd_, 400, "Bad Request: Lock of Content-length");
                break;
            }
            if (static_cast<int>(buffer_in_.size() < content_length)) break;
            process_state_ = STATE_ANALYSIS;
        }
        if (process_state_ == STATE_ANALYSIS) {
            AnalysisState flag = this->analyze_request();
            if (flag == ANALYSIS_SUCCESS) {
                process_state_ = STATE_FINISH;
                break;
            }
            else {
                is_error_ = true;
                break;
            }
        }
    } while (false);
    if (!is_error_) {
        if (buffer_out_.size() > 0) handle_write();
    }
    if (!is_error_ && process_state_ == STATE_FINISH) {
        this->reset();
        if (buffer_in_.size() > 0) {
            if (connection_state_ != H_DISCONNECTED) handle_read();
        }
    }
    else if (!is_error_ && connection_state_ != H_DISCONNECTED) events_ |= EPOLLIN;
}

void HttpData::handle_write() {
    if (!is_error_ && connection_state_ != H_DISCONNECTED) {
        __uint32_t &events_ = channel_->get_events();
        if (writen(fd_, buffer_out_) < 0) {
            perror("writen");
            events_ = 0;
            is_error_ = true;
        }
        if (buffer_out_.size() > 0) events_|= EPOLLOUT;
    }
}

void HttpData::handle_connect() {
    seperate_timer();
    __uint32_t &events_ = channel_->get_events();
    if (!is_error_ && connection_state_ == H_CONNECTED) {
        if (events_ != 0) {
            int timeout = DEFAULT_EXPIRED_TIME;
            if (keep_alive_) timeout = DEFAULT_KEEP_ALIVE_TIME;
            if ((events_ & EPOLLIN) && (events_ & EPOLLOUT)) {
                events_ = __uint32_t(0);
                events_ |= EPOLLOUT;
            }
            events_ |= EPOLLET;
            event_loop_->update_poller(channel_, timeout);
        }
        else if (keep_alive_) {
            events_ |= (EPOLLIN || EPOLLET);
            int timeout = DEFAULT_KEEP_ALIVE_TIME;
            event_loop_->update_poller(channel_, timeout);
        }
        else {
            event_loop_->shutdown(channel_);
            event_loop_->run_in_loop(bind(&HttpData::handle_close, shared_from_this()));
            events_ |= (EPOLLIN | EPOLLET);
            int timeout = (DEFAULT_KEEP_ALIVE_TIME >> 1);
            event_loop_->update_poller(channel_, timeout);
        }
    }
    else if (!is_error_ && connection_state_ == H_DISCONNECTED && (events_ & EPOLLOUT)) {
        events_ = (EPOLLOUT | EPOLLET);
    }
    else event_loop_->run_in_loop(bind(&HttpData::handle_close, shared_from_this()));
}

URIState HttpData::parse_URI() {
    string &string1 = buffer_in_;
    string string2 = string1;
    // read the request line
    size_t cur = string1.find('\r', read_cur_);
    if (cur < 0) return PARSE_URI_AGAIN;
    // remove the request line to save the space.
    string request_line = string1.substr(0, cur);
    if (string1.size() > cur + 1) string1 = string1.substr(cur + 1);
    else string1.clear();

    // Http method
    int cur_get = request_line.find("GET");
    int cur_post = request_line.find("POST");
    int cur_head = request_line.find("HEAD");

    if (cur_get >= 0) {
        cur = cur_get;
        Http_method_ = METHOD_GET;
    }
    else if (cur_post >= 0) {
        cur = cur_post;
        Http_method_ = METHOD_POST;
    }
    else if (cur_head >= 0) {
        cur = cur_head;
        Http_method_ = METHOD_HEAD;
    }
    else return PARSE_URI_ERROR;

    // URI
    cur = request_line.find("/", cur);
    if (cur < 0) {
        file_name_ = "index.html";
        Http_version_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    }
    else {
        size_t _cur = request_line.find(' ', cur);
        if (_cur < 0) return PARSE_URI_ERROR;
        else {
            if (_cur - cur > 1) {
                file_name_ = request_line.substr(cur + 1, _cur - cur - 1);
                size_t __cur = file_name_.find('?');
                if (__cur >= 0) file_name_ = file_name_.substr(0, __cur);
            }
            else file_name_ = "index.html";
        }
        cur = _cur;
    }
    cur = request_line.find('/', cur);
    if (cur < 0) return PARSE_URI_ERROR;
    else if (request_line.size() - cur <= 3) return PARSE_URI_ERROR;
    else {
        string version = request_line.substr(cur + 1, 3);
        if (version == "1.0") Http_version_ = HTTP_10;
        else if (version == "1.1") Http_version_ = HTTP_11;
        else return PARSE_URI_ERROR;
    }
    return PARSE_URI_SUCCESS;
}

HeaderState HttpData::parse_Headers() {
    string &string1 = buffer_in_;
    int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
    int read_cur = 0;
    bool finished = false;
    for (size_t i = 0; i < string1.size() && !finished; i++) {
        switch (parse_state_) {
            case H_START: {
                if (string1[i] == '\n' || string1[i] == 'r') break;
                parse_state_ = H_KEY;
                key_start = i;
                read_cur = i;
                break;
            }
            case H_KEY: {
                if (string1[i] == ':') {
                    key_end = i;
                    if (key_end - key_start <= 0) return PARSE_HEADER_ERROR;
                    parse_state_ = H_COLON;
                }
                else if (string1[i] == '\n' || string1[i] == '\r') return PARSE_HEADER_ERROR;
                break;
            }
            case H_COLON: {
                if (string1[i] == ' ') parse_state_ = H_SPACES_AFTER_COLON;
                else return PARSE_HEADER_ERROR;
                break;
            }
            case H_SPACES_AFTER_COLON: {
                parse_state_ = H_VALUE;
                value_start = i;
                break;
            }
            case H_VALUE: {
                if (string1[i] == '\r') {
                    parse_state_ = H_CR;
                    value_end = i;
                    if (value_end - value_start <= 0) return PARSE_HEADER_ERROR;
                }
                // overflow the max length limitation of value.
                else if (i - value_start > 255) return PARSE_HEADER_ERROR;
                break;
            }
            case H_CR: {
                if (string1[i] == '\n') {
                    parse_state_ = H_LF;
                    string key(string1.begin() + key_start, string1.begin() + key_end);
                    string value(string1.begin() + value_start, string1.begin() + value_end);
                    headers_[key] = value;
                    read_cur = i;
                }
                else return PARSE_HEADER_ERROR;
                break;
            }
            case H_LF: {
                if (string1[i] == '\r') parse_state_ = H_END_CR;
                else {
                    key_start = i;
                    parse_state_ = H_KEY;
                }
                break;
            }
            case H_END_CR: {
                if (string1[i] == '\n') parse_state_ = H_END_LF;
                else return PARSE_HEADER_ERROR;
                break;
            }
            case H_END_LF: {
                finished = true;
                key_start = i;
                read_cur = i;
                break;
            }
        }
    }
    if (parse_state_ == H_END_LF) {
        string1 = string1.substr(i);
        return PARSE_HEADER_SUCCESS;
    }
    string1 = string1.substr(read_cur);
    return PARSE_HEADER_AGAIN;
}

AnalysisState HttpData::analyze_request() {
    if (Http_method_ == METHOD_POST) {
         string header;
         header += string("HTTP/1.1 200 OK\r\n");
         if(headers_.find("Connection") != headers_.end() && headers_["Connection"] == "Keep-Alive") {
             keep_alive_ = true;
             header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout="
                     + to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
         }
         int length = stoi(headers_["Content-length"]);
         vector<char> data(buffer_in_.begin(), buffer_in_.begin() + length);
         Mat source = imdecode(data, CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);
         //imwrite("receive.bmp", src);
         Mat result = stitch(source);
         vector<uchar> data_encode;
         imencode(".png", result, data_encode);
         header += string("Content-length: ") + to_string(data_encode.size()) + "\r\n\r\n";
         buffer_in_ += header + string(data_encode.begin(), data_encode.end());
         buffer_in_ = buffer_in_.substr(length);
         return ANALYSIS_SUCCESS;
    }
    else if (Http_method_ == METHOD_GET || Http_method_ == METHOD_HEAD) {
        string header;
        header += "HTTP/1.1 200 OK\r\n";
        if (headers_.find("Connection") != headers_.end() && (headers_["Connection"] == "Keep-Alive" ||
            headers_["Connection"] == "keep-alive")) {

        }
    }
}