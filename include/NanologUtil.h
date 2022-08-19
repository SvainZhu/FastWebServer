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
#include <stdarg.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

#include "Portability.h"

using namespace std;

namespace NanologUtil {
    string hex_dump(const void *buffer, uint64_t bytes);
    static NANOLOG_ALWAYS_INLINE uint64_t rdpmc(int ecx);
    static NANOLOG_ALWAYS_INLINE pid_t gettid();
    static NANOLOG_ALWAYS_INLINE void pin_thread_to_core(int id);
    static NANOLOG_ALWAYS_INLINE cpu_set_t get_cpu_affinity();
    static NANOLOG_ALWAYS_INLINE void set_cpu_affinity(cpu_set_t cpuset);
    static NANOLOG_ALWAYS_INLINE void serialize();
    static NANOLOG_ALWAYS_INLINE void barrier();
    static NANOLOG_ALWAYS_INLINE uint64_t serial_read_pmc(int ecx);

    // the size of cache line (bytes)
    static const uint32_t BYTES_PER_CACHE_LINE = 64;

    // return the numbers of elements in static array.
    template<class T, size_t N>
    constexpr size_t array_size(T (&)[N]) {return N;}
}


#endif //FASTWEBSERVER_NANOLOGUTIL_H
