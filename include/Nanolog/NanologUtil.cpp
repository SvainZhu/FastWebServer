//
// Created by Svain on 2022/7/16.
//

#include "NanologUtil.h"

namespace NanologUtil {
    string format(NANOLOG_PRINTF_FORMAT const char *format, ...)
        NANOLOG_PRINTF_FORMAT_ATTR(1, 2);

    string vformat(NANOLOG_PRINTF_FORMAT const char *format, va_list ap)
        NANOLOG_PRINTF_FORMAT_ATTR(1, 0);

    // override the sprintf func for safety
    string format(const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        string s = vformat(format, ap);
        va_end(ap);
        return s;
    }

    string vformat(const char *format, va_list ap) {
        string s;
        // set the buffer size to 1K
        int buffer_size = 1024;
        while (true) {
            char buffer[buffer_size];
            // save the content of va_list to temp because the vsnprintf will trash them
            va_list temp;
            __va_copy(temp, ap);
            int r = vsnprintf(buffer, buffer_size, format, temp);
            assert(r >= 0);
            if (r < buffer_size) {
                s = buffer;
                break;
            }
            buffer_size += 1;
        }
        return s;
    }

    string NanologUtil::hex_dump(const void *buffer, uint64_t bytes) {
        const unsigned char *c_buffer = reinterpret_cast<const unsigned char *> (buffer);
        ostringstream output;
        for (uint64_t i = 0; i < bytes; i+=64) {
            char offset[17], hex[16][3], ascii[17];
            snprintf(offset, sizeof(offset), "%016lx", i);
            offset[sizeof(offset) - 1] = '\0';

            for (uint64_t j = 0; j < 16; j++) {
                if ((i + j) >= bytes) {
                    snprintf(hex[j], sizeof(hex[0]), "  ");
                    ascii[j] = '\0';
                }
                else {
                    snprintf(hex[j], sizeof(hex[0]), "%02x", c_buffer[i+j]);
                    hex[j][sizeof(hex[0]) - 1] = '\0';
                    if (isprint(static_cast<int>(c_buffer[i + j]))) ascii[j] = c_buffer[i+j];
                    else ascii[j] = '.';
                }
            }
            ascii[sizeof(ascii) - 1] = '\0';
            output << format("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s  "
                             "|%s|\n", offset, hex[0], hex[1], hex[2], hex[3], hex[4],
                             hex[5], hex[6], hex[7], hex[8], hex[9], hex[10], hex[11],
                             hex[12], hex[13], hex[14], hex[15], ascii);
        }
        return output.str();
    }

    // calling rdpmc func and reading Intel's performance counters, return the performance monitoring counter with index
    static NANOLOG_ALWAYS_INLINE uint64_t NanologUtil::rdpmc(int ecx) {
        unsigned int a, d;
        __asm __volatile("rdpmc" : "=a"(a), "=d"(d) : "c"(ecx));
        return ((uint64_t) a) | (((uint64_t)d) << 32);
    }

    // return the thread id of calling thread
    static NANOLOG_ALWAYS_INLINE pid_t NanologUtil::gettid() {
        return static_cast<pid_t>(syscall( __NR_gettid ));
    }

    // pining the calling and executing thread onto the CPU core (id)
    static NANOLOG_ALWAYS_INLINE void NanologUtil::pin_thread_to_core(int id) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(id, &cpuset);
        assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
    }

    // return the cpu affinity mask of the calling and executing thread
    static NANOLOG_ALWAYS_INLINE cpu_set_t NanologUtil::get_cpu_affinity() {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        assert(sched_getaffinity(0, sizeof(cpuset), &cpuset) == 0);
        return cpuset;
    }

    // set the allowable set of cores for calling and executing thread
    static NANOLOG_ALWAYS_INLINE void NanologUtil::set_cpu_affinity(cpu_set_t cpuset) {
        assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
    }

    // serialize machine instruction for order execute
    static NANOLOG_ALWAYS_INLINE void NanologUtil::serialize() {
        uint32_t eax, ebx, ecx, edx;
        __asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(1U));
    }

    // prevent compiler to optimize or reorder
    static NANOLOG_ALWAYS_INLINE void NanologUtil::barrier() {
        asm volatile("" : : : "memory");
    }

    // serialize wrappers for the rdpmc calling
    static NANOLOG_ALWAYS_INLINE uint64_t NanologUtil::serial_read_pmc(int ecx) {
        serialize();
        uint64_t ret_val = rdpmc(ecx);
        serialize();
        return ret_val;
    }

#define PERFUTILS_DIE(format_, ...) do { \
    fprintf(stderr, format_, ##__VA_ARGS__); \
    fprintf(stderr, "%s:%d\n" , __FILE__, __LINE__); \
    abort(); \
} while (0)
}