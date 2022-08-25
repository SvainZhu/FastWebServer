//
// Created by Svain on 2022/8/18.
//

#ifndef FASTWEBSERVER_CYCLES_H
#define FASTWEBSERVER_CYCLES_H

#include <stdint.h>
#include <errno.h>
#include <sys/time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "Portability.h"

class Cycles {
public:
    static void init();
    static NANOLOG_ALWAYS_INLINE uint64_t rdtsc();
    static NANOLOG_ALWAYS_INLINE double per_second();
    static double to_seconds(int64_t cycles, double cycles_per_second = 0);
    static uint64_t from_seconds(double seconds, double cycles_per_second = 0);
    static uint64_t to_nanoseconds(int64_t cycles, double cycles_per_second = 0);
    static uint64_t from_nanoseconds(double nanoseconds, double cycles_per_second = 0);
    static void sleep(uint64_t ns);
    static NANOLOG_ALWAYS_INLINE double get_cycles_per_second();

private:
    Cycles();
    double cycles_per_second;
    // Testing: return the result of the next call rdtsc()
    static uint64_t mock_tsc_value;
    // Testing: return the result that convert from cycles to seconds
    static double mock_cycles_per_second;

};

#endif //FASTWEBSERVER_CYCLES_H
