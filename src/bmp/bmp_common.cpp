#include "bmp_common.hpp"

#include "bits.hpp"

namespace BMP {

// TODO: Unit tests
float2_30_t uint32_to_float2_30(uint32_t value)
{
    float2_30_t output_value;
    output_value.value = value;
    return output_value;
}

// TODO: Unit tests
uint32_t float2_30_to_uint32(float2_30_t value)
{
    return value.value;
}

Bitmask::Bitmask(uint32_t value) :
    value { value },
    offset { first_bit_set(value) },
    divider { generate_bitmask<double>(count_set_bits(value)) }
{
}

}
