#include "bits.hpp"

uint8_t reverse_byte(uint8_t value)
{
    value = (value & 0b11110000) >> 4 | (value & 0b00001111) << 4;
    value = (value & 0b11001100) >> 2 | (value & 0b00110011) << 2;
    value = (value & 0b10101010) >> 1 | (value & 0b01010101) << 1;
    return value;
}
