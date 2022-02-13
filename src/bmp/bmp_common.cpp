#include "bmp_common.hpp"

#include "bits.hpp"

// TODO: Unit tests
BMP::float2_30_t BMP::uint32_to_float2_30(uint32_t value)
{
    float2_30_t output_value;
    output_value.value = value;
    return output_value;
}

// TODO: Unit tests
uint32_t BMP::float2_30_to_uint32(BMP::float2_30_t value)
{
    return value.value;
}

BMP::Bitmask::Bitmask(uint32_t value) :
    value(value),
    offset(first_bit_set(value)),
    divider(generate_bitmask<double>(count_set_bits(value)))
{
}
