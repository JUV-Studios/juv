#ifndef JUV_FRACTION
#define JUV_FRACTION
#include "numeric_cast.hpp"
#ifdef __has_include(<winrt/impl/Windows.Foundation.Numerics.0.h>)
#include <winrt/base.h>
#include <winrt/impl/Windows.Foundation.Numerics.0.h>
#endif

namespace juv
{
	template <std::integral T>
	struct fraction
	{
		T numerator, denominator;

		fraction() = default;

		// TODO disallow implicit conversions from float.
		constexpr fraction(T top, T bottom = 1) noexcept : numerator{ top }, denominator{ bottom } {}

	#ifdef WINRT_Windows_Foundation_Numerics_0_H
		using native_type = winrt::Windows::Foundation::Numerics::Rational;

		constexpr fraction(native_type native) : numerator{ native.Numerator }, denominator{ native.Denominator } {}
	#endif
	};
}

#endif