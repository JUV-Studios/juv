#ifndef JUV_WITH_FLEXIBLE_ARRAY
#define JUV_WITH_FLEXIBLE_ARRAY

#include "macros.hpp" // for JUV_DERIVED
#include <utility> // for std::as_const

namespace juv
{
	template <typename Derived, typename T>
	struct with_flexible_array
	{
		[[nodiscard]]
		auto flexible_array() const noexcept
		{
			struct test
			{
				std::byte object[sizeof(Derived)];
				T fam_first;
			};

			static_assert(sizeof(test) - sizeof(T) - sizeof(Derived) == trailing_padding());
			return reinterpret_cast<T const*>(reinterpret_cast<std::byte const*>(&JUV_DERIVED + 1) + trailing_padding());
		}

		[[nodiscard]] auto flexible_array() noexcept { return const_cast<T*>(std::as_const(*this).flexible_array()); }
		
		[[nodiscard]]
		static size_t consteval trailing_padding()
		{
			return alignof(Derived) < alignof(T) ? alignof(T) - alignof(Derived) : 0;
		}

		[[nodiscard]]
		static size_t constexpr allocation_size(size_t tcount) noexcept
		{
			return sizeof(Derived) + trailing_padding() + tcount * sizeof(T);
		}
	};
}

#endif