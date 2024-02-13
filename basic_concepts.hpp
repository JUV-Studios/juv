#ifndef JUV_BASIC_CONCEPTS
#define JUV_BASIC_CONCEPTS

#include "macros.hpp"
#include <concepts>

namespace juv
{
	template <typename T, typename... U>
	concept same_as_any = (std::same_as<T, U> || ...);

	template <typename From, typename... To>
	concept convertible_to_any = (std::convertible_to<From, To> || ...);

	// Reference: https://en.cppreference.com/w/cpp/named_req/NullablePointer.
	// Note: the NullablePointer named requirements states that a value-initialised object of the type must produce a null value.w  Not only that we can't check for this with a concept, but some of our types also violate this.
	template <typename T>
	concept nullable_pointer = requires
	{
		{ std::convertible_to<std::nullptr_t, T> };
		requires requires(T&& t) { static_cast<bool>(t); t == nullptr; };
	};
}

#endif