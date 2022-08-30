#include <iostream>

#include <string>
#include <getopt.h>
#include "EventLoop.h"
#include "include/Log/Logger.h"
#include "Server.h"

using namespace std;

int main(int argc, char *argv[]) {
    int num_threads = 4;
    int port = 80;
    string log_path = "./FastWebServer.log";

    int opt;
    const char *string1 = "t:l:p:";
    opt = getopt(argc, argv, string1);
    while (opt != -1) {
        switch (opt) {
            case 't': {
                num_threads = atoi(optarg);
                break;
            }
            case 'l': {
                log_path = optarg;
                if (log_path.size() < 2 || optarg[0] != '/') {
                    printf("Log path is invalid");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }
    Logger::set_log_name(log_path);
#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined!!!";
#endif
    EventLoop main_event_loop;
    Server web_server(&main_event_loop, num_threads, port);
    web_server.start();
    main_event_loop.loop();
    return 0;
}
