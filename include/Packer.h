//
// Created by Svain on 2022/7/22.
//

#ifndef FASTWEBSERVER_PACKER_H
#define FASTWEBSERVER_PACKER_H

#include <cassert>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <type_traits>

#include "Portability.h"

using namespace std;

/* To pack/unpack for integer value with the fewest number of bytes (compress), but no compress for float and string.
 * Check the integer and determine the bytes number for storing, and bytes number and the sign bit will be store in a
 * 4-bit code called by nibble. The nibble will be generated by the pack() and passed back by the unpack().
 * The pack()/unpack() will only correct on little-endian machines.
 * The pack()/unpack() require the caller to explicitly specify the type of variable. The nanolog system will encode the
 * types directly into generated code.
 * The value of the special code S represents by :
 * 0                        -> 16-byte value
 * [1, sizeof(T)]           -> S bytes integer
 * [9, 8+sizeof(T)]         -> S-8 bytes negative integer
*/

// TODO To encode number deltas instead of smallest value is best for log metrics with monotonically increasing (numbers)

namespace NanologBufferUtil {

    // Pack two 4 bits nibbles into 1 bytes.
    NANOLOG_PACK_PUSH struct two_nibbles {
        uint8_t first:4;
        uint8_t second:4;
    }; NANOLOG_PACK_POP

    // Pack an unsigned integer and a char array, determine the fewest byte number of the integer and copy that into char array.
    // binary search for the fewest bytes container to store integer.
    template<typename T>
    inline typename enable_if<is_integral<T>::value && !is_signed<T>::value, int>::type pack(char **buffer, T value) {
        int num_bytes = 0;
        if (value < (1UL << 8)) {
            num_bytes = 1;
        }
        else if (value < (1UL << 16)) {
            num_bytes = 2;
        }
        else if (value < (1UL << 24)) {
            num_bytes = 3;
        }
        else if (value < (1UL << 32)) {
            num_bytes = 4;
        }
        else if (value < (1UL << 40)) {
            num_bytes = 5;
        }
        else if (value < (1UL << 48)) {
            num_bytes = 6;
        }
        else if (value < (1UL << 56)) {
            num_bytes = 7;
        }
        else {
            num_bytes = 8;
        }
        memcpy(*buffer, &value, sizeof(T));
        *buffer += num_bytes;
        return num_bytes;
    }

    // pack() for packing a signed integer into the unsigned version
    inline int pack(char **buffer, int32_t value) {
        if (value >= 0 || value <= int32_t(-(1<<24))) return pack<uint32_t>(buffer, static_cast<uint32_t>(value));
        else return 8 + pack<uint32_t>(buffer, static_cast<uint32_t>(-value));
    }

    inline int pack(char **buffer, int64_t value) {
        if (value >= 0 || value <= int64_t(-(1LL<<56))) return pack<uint64_t>(buffer, static_cast<uint64_t>(value));
        else return 8 + pack<uint64_t>(buffer, static_cast<uint64_t>(-value));

    }

    inline int pack(char **buffer, long long int value) {
        if (value >= 0 || value <= int64_t(-(1LL<<56))) return pack<uint64_t>(buffer, static_cast<uint64_t>(value));
        else return 8 + pack<uint64_t>(buffer, static_cast<uint64_t>(-value));
    }

    // The printf doesn't an int16_t or int8_t, so there is no type information to unpack them based purely on the format string.
#if 0
    inline int pack(char **buffer, int16_t) {
        if (value >= 0 || value <= int16_t(-(1<<8))) return pack<uint16_t>(buffer, static_cast<uint16_t>(value));
        else return 8 + pack<uint16_t>(buffer, static_cast<uint16_t>(-value));
    }

    inline int pack(char **buffer, int8_t value) {
        **buffer = value;
        (*buffer)++;
        return 1;
    }
#endif

    // pack() template only copy the pointer and float number without compression.
    template<typename T>
    inline int pack(char **buffer, T* pointer) {
        return pack<uint64_t>(buffer, reinterpret_cast<uint64_t>(pointer));
    }

    template<typename T>
    inline typename enable_if<is_floating_point<T>::value, int>::type pack(char **buffer, T value) {
        memcpy(*buffer, &value, sizeof(T));
        *buffer += sizeof(T);
        return sizeof(T);
    }

    // unpack the pack() data package that take in a data array pointer and the special pack() code S
    template<typename T>
    inline typename enable_if<!is_floating_point<T>::value && !is_pointer<T>::value, T>::type
        unpack(const char **data_array, uint8_t pack_code) {
        int64_t package = 0;
        if (pack_code <= 8) {
            memcpy(&package, *data_array, pack_code);
            *data_array += pack_code;
            return static_cast<T>(package);
        }
        else {
            int num_bytes = pack_code == 0 ? 16 : (0x0f & (pack_code - 8));
            memcpy(&package, *data_array, num_bytes);
            *data_array += num_bytes;
            return static_cast<T>(-package);
        }
    }

    // unpack the pack() data package with pointer or float number.
    template<typename T>
    inline typename enable_if<is_pointer<T>::value, T>::type unpack(const char **data_array, uint8_t pack_nibble) {
        return (T*)(unpack<uint64_t>(data_array, pack_nibble));
    }

    template<typename T>
    inline typename enable_if<is_floating_point<T>::value, T>::type unpack(const char **data_array, uint8_t pack_nibble) {
        if (pack_nibble == sizeof(float)) {
            float package;
            memcpy(&package, *data_array, sizeof(float));
            *data_array += sizeof(float);
            return package;
        }
        else {
            T package;
            int num_bytes = pack_nibble == 0 ? 16 : pack_nibble;
            memcpy(&package, *data_array, num_bytes);
            *data_array += num_bytes;
            return package;
        }
    }

    // return the total number of bytes in a data package encoded with a stream of nibbles
    /* \param nibbles: the start pointer of the nibbles
     * \param num_nibbles: the number of nibbles of the nibbles stream
     * \return: the totol size of bytes in that data package
     */
    inline static uint32_t get_size_of_package(const two_nibbles *nibbles, uint32_t num_nibbles) {
        uint32_t size = 0;
        for (uint32_t i = 0; i < num_nibbles/2; i++) {
            size += nibbles[i].first + nibbles[i].second;
            if (nibbles[i].first == 0) size += 16;
            if (nibbles[i].first > 0x8) size -= 8;
            if (nibbles[i].second == 0) size += 16;
            if (nibbles[i].second > 0x8) size -= 8;
        }

        if (num_nibbles & 0x1) {
            size += nibbles[num_nibbles / 2].first;
            if (nibbles[num_nibbles / 2].first == 0) size += 16;
            if (nibbles[num_nibbles / 2].first > 0x8) size -= 8;
        }
        return size;
    }

    class Nibbler {
    public:
        Nibbler(const char *nibble_start, int num_nibbles) :
            nibble_position(reinterpret_cast<const two_nibbles*>(nibble_start)), is_first_nibble(true),
            num_nibbles(num_nibbles), current_package(nibble_start + (num_nibbles + 1) / 2), end_of_package(nullptr) {
            end_of_package = nibble_start + (num_nibbles + 1)/2 + get_size_of_package(nibble_position, num_nibbles);
        }

        // return next package in the data stream
        template<typename T>
        T get_next() {
            assert(current_package < end_of_package);
            uint8_t nibble = is_first_nibble ? nibble_position->first : nibble_position->second;
            T next = unpack<T>(&current_package, nibble);
            if (!is_first_nibble) nibble_position ++;
            is_first_nibble != is_first_nibble;
            return next
        }

        // get the end of package
        const char *get_end_of_package() {
            return end_of_package;
        }

    private:
        // the position in nibble stream
        const two_nibbles *nibble_position;
        // indicate whether to use the first or second nibble
        bool is_first_nibble;
        // the number of nibbles in stream
        int num_nibbles;
        // marking the next data package
        const char *current_package;
        // end of last data package
        const char *end_of_package;

    };
}

#endif //FASTWEBSERVER_PACKRE_H
