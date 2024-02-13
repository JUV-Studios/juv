#ifndef JUV_SAFE_NUMERICS
#define JUV_SAFE_NUMERICS
#include <concepts>
#include <limits>
#include <stdexcept>

namespace juv
{
	template <std::integral To>
	To constexpr numeric_cast(std::integral auto from)
	{
		if constexpr (sizeof(To) < sizeof(decltype(from)))
		{
			if (from > std::numeric_limits<To>::max()) throw std::invalid_argument{ "Value of 'from' could not be converted to type To, as it is too large." };
			else if (from < std::numeric_limits<To>::min()) throw std::invalid_argument{ "Value of 'from' could not be converted to type To, as it is too small." };
		}

		return static_cast<To>(from);
	}
}

#endif