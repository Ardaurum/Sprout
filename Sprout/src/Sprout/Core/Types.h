#pragma once

#include <cstdint>

// 128 bit uint
typedef struct
{
    uint64_t low64;
    uint64_t high64;
} uint128_sprout_t;