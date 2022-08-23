//
// Created by Svain on 2022/8/23.
//

#ifndef FASTWEBSERVER_NANOLOG_H
#define FASTWEBSERVER_NANOLOG_H

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <utility>

#include "Cycles.h"
#include "Portability.h"
#include "Packer.h"

namespace NanologInternal {
    // check whether the char is in the terminal set of format specifier characters
    constexpr inline bool is_terminal(char c) {
        return  c == 'd' || c == 'i' || c == 'u' || c == 'o' || c == 'x' || c == 'X' || c == 'f' || c == 'F'
                || c == 'e' || c == 'E' || c == 'g' || c == 'G' || c == 'a' || c == 'A' || c == 'c' || c == 'p'
                || c == '%' || c == 's' || c == 'n';
    }

    // check whether the char is in the flag set of format specifier characters
    constexpr inline bool is_flag(char c) {
        return c == '+' || c == '-' || c == '0' || c == '#' || c == ' ';
    }

    // check whether the char is in the set of a length field
    constexpr inline bool is_length(char c) {
        return c == 'h' || c == 'l' || c == 'j' || c == 'z' ||  c == 't' || c == 'L';
    }

    // check whether the char is a digit
    constexpr inline bool is_digit(char c) {
        return (c >= '0' && c <= '9');
    }

    /* analyze a static printf style format string and extract type information.
     * \tparam N: the length of the static format string.
     * \param static_format_string: static format string.
     * \param num_param: the number of parameter to return the type information
     * \return: an ParamType enum of parameter type information.
     */
    template<int N>
    constexpr inline ParamType get_param_type(const char (&static_format_string)[N], int num_param = 0) {
        int position = 0;
        while (position < N - 1) {
            if (static_format_string[position] != '%') {
                position++;
                continue;
            }
            else {
                position++;
                if (static_format_string[position] == '%') {
                    position++;
                    continue;
                }
                else {
                    while (is_flag(static_format_string[position])) position++;
                    if
                }
            }
        }
    }
}


#endif //FASTWEBSERVER_NANOLOG_H
