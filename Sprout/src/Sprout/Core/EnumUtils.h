#pragma once

#include <type_traits>

template<typename T>
constexpr auto EnumValue(T e) -> typename std::underlying_type_t<T>
{
	return static_cast<std::underlying_type_t<T>>(e);
}

#define MAKE_ENUM_FLAG(enum_type) \
inline enum_type operator|(enum_type lhs, enum_type rhs) \
{ \
	using T = std::underlying_type_t<enum_type>; \
	return static_cast<enum_type>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
} \
\
inline enum_type& operator|=(enum_type& lhs, enum_type rhs) \
{ \
	lhs = lhs | rhs; \
	return lhs; \
} \
\
inline enum_type operator&(enum_type lhs, enum_type rhs) \
{ \
	using T = std::underlying_type_t<enum_type>; \
	return static_cast<enum_type>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
} \
\
inline enum_type& operator&=(enum_type& lhs, enum_type rhs) \
{ \
	lhs = lhs & rhs; \
	return lhs; \
} \
\
inline enum_type operator^(enum_type lhs, enum_type rhs) \
{ \
	using T = std::underlying_type_t<enum_type>; \
	return static_cast<enum_type>(static_cast<T>(lhs) ^ static_cast<T>(rhs)); \
} \
\
inline enum_type& operator^=(enum_type& lhs, enum_type rhs) \
{ \
	lhs = lhs ^ rhs; \
	return lhs; \
} \
\
inline enum_type operator~(enum_type val) \
{ \
	using T = std::underlying_type_t<enum_type>; \
	return static_cast<enum_type>(~static_cast<T>(val)); \
} \
inline bool Any(enum_type val) \
{ \
	return val != enum_type::None; \
}