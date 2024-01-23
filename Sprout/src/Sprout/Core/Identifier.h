#pragma once

#include "HashUtils.h"
#include <string>

using ID32 = uint32_t;
using ID64 = uint64_t;

static constexpr ID32 const Identifier32(char const* id, uint32_t const seed = 0) { return xxh32::hash(id, static_cast<uint32_t>(std::char_traits<char>::length(id)), seed); }
static constexpr ID64 const Identifier64(char const* id, uint64_t const seed = 0) { return xxh64::hash(id, static_cast<uint64_t>(std::char_traits<char>::length(id)), seed); }