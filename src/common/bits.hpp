#pragma once

#include <cmath>
#include <cstdint>

#define BYTE 8

/*
 * Count the number of bits equals to 1 in the value.
 */
template <typename T>
uint8_t count_set_bits(T value)
{
    uint8_t count = 0;
    uint8_t size = sizeof(value) * BYTE;
    for (uint8_t i = 0; i<size; ++i, value = value>> 1)
        if ((value & 1) == 1) ++count;
    return count;
}

template <typename T>
uint8_t first_bit_set(T value)
{
    return log2((~value + 1) & value);
}

template <typename T>
T generate_bitmask(uint8_t number_of_set_bits)
{
    return (1 << number_of_set_bits) - 1;
}

uint8_t reverse_byte(uint8_t value);
