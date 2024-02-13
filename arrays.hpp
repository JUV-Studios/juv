#ifndef JUV_ARRAYS
#define JUV_ARRAYS

#include "wrapperers.hpp"
#include <memory>

namespace juv
{
	namespace impl
	{
		template <typename Pointer>
		struct dynarray_base : array_view_base<dynarray_base<Pointer>>
		{
		public:
			dynarray_base() = default;

			dynarray_base(Pointer data, size_t size) noexcept : m_ptr{ std::move(data) }, m_size{ m_ptr ? size : 0 } {}

			auto& operator& () const noexcept { return m_ptr; }

			auto operator& () && noexcept
			{
				m_size = 0;
				return std::move(m_ptr);
			}
		private:
			Pointer m_ptr = nullptr;
			size_t m_size = 0;
		};
	}

	#define JUV_IMPL_UNIQUE_ARRAY_BASE impl::dynarray_base<std::unique_ptr<T[], Deleter>>

	template <typename T, typename Deleter = std::default_delete<T[]>>
	struct unique_array : JUV_IMPL_UNIQUE_ARRAY_BASE
	{
		using JUV_IMPL_UNIQUE_ARRAY_BASE::dynarray_base;
	};

	#define JUV_IMPL_SHARED_ARRAY_BASE impl::dynarray_base<std::shared_ptr<T[]>>

	template <typename T>
	struct shared_array : JUV_IMPL_SHARED_ARRAY_BASE
	{
		using JUV_IMPL_SHARED_ARRAY_BASE::dynarray_base;
	};
}

#endif