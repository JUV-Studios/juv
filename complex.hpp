#ifndef JUV_COMPLEX
#define JUV_COMPLEX

#include <complex>
#include <concepts>

namespace juv
{
	template <std::floating_point T>
	using complex = std::complex<T>;
}

#endif