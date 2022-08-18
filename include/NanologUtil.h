//
// Created by Svain on 2022/7/16.
//

#ifndef FASTWEBSERVER_NANOLOGUTIL_H
#define FASTWEBSERVER_NANOLOGUTIL_H

#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <stdint.h>
#include <sched.h>
#include <cassert>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

#include "Portability.h"

using namespace std;

namespace NanologUtil {
    string hexDump(const void *buffer, uint64_t bytes);
    static NANOLOG_ALWAYS_INLINE uint64_t rdpmc(int ecx);
}


#endif //FASTWEBSERVER_NANOLOGUTIL_H
