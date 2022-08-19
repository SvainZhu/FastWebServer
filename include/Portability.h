//
// Created by Svain on 2022/7/16.
//
#include <sal.h>

#ifndef FASTWEBSERVER_PORTABILITY_H
#define FASTWEBSERVER_PORTABILITY_H
// MSVC env
#ifdef _MSC_VER
#define NANOLOG_ALWAYS_INLINE __forceinline
#define NANOLOG_NOINLINE __declspec(noinline)
#define NANOLOG_PACK_PUSH __pragma(pack(push, 1))
#define NANOLOG_PACK_POP __pragma(pack(pop))

#define _USE_ATTRIBUTES_FOR_SAL 1

#define NANOLOG_PRINTF_FORMAT _Printf_format_string_
#define NANOLOG_PRINTF_FORMAT_ATTR(string_index, first_to_check)
// GNU C
#elif defined(__GNUC__)
#define NANOLOG_ALWAYS_INLINE inline __attribute__((__always_inline__))
#define NANOLOG_NOINLINE __attribute__((__noinline__))
#define NANOLOG_PACK_PUSH _Pragma("pack(push, 1)")
#define NANOLOG_PACK_POP _Pragma("pack(pop)")

#define NANOLOG_PRINTF_FORMAT
#define NANOLOG_PRINTF_FORMAT_ATTR(string_index, first_to_check) \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
// Other
#else
#define NANOLOG_ALWAYS_INLINE inline
#define NANOLOG_NOINLINE
#define NANOLOG_PACK_PUSH
#define NANOLOG_PACK_POP

#define NANOLOG_PRINTF_FORMAT
#define NANOLOG_PRINTF_FORMAT_ATTR(string_index, first_to_check)

#endif //FASTWEBSERVER_PORTABILITY_H
