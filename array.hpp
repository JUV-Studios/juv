#ifndef JUV_ARRAYS
#define JUV_ARRAYS

#include "wrapperers.hpp"
#include <memory>

namespace juv
{
	namespace impl
	{
		template <typename Pointer>
		struct dynarray_base : array_view_base<dynarray_base<Pointer>, Pointer::element_type, size_t>
		{
		public:
			dynarray_base() = default;

			dynarray_base(Pointer data, size_t size) noexcept : m_ptr{ std::move(data) }, m_size{ m_ptr ? size : 0 } {}

			auto& operator& () const& noexcept { return m_ptr; }

			auto operator& () && noexcept
			{
				m_size = 0;
				return std::move(m_ptr);
			}

			auto data() const noexcept { return m_ptr.get(); }
			auto size() const noexcept { return m_size; }
		private:
			Pointer m_ptr = nullptr;
			size_t m_size = 0;
		};
	}

	#define JUV_IMPL_UNIQUE_ARRAY_BASE impl::dynarray_base<std::unique_ptr<T[], Deleter>>

	template <typename T, typename Deleter = std::default_delete<T[]>>
	struct unique_array final : JUV_IMPL_UNIQUE_ARRAY_BASE
	{
		using JUV_IMPL_UNIQUE_ARRAY_BASE::dynarray_base;
		explicit unique_array(size_t size) : dynarray_base{ std::make_unique_for_overwrite<T[]>(size) } {}
	};

	#define JUV_IMPL_SHARED_ARRAY_BASE impl::dynarray_base<std::shared_ptr<T[]>>

	template <typename T>
	struct shared_array final : JUV_IMPL_SHARED_ARRAY_BASE
	{
		using JUV_IMPL_SHARED_ARRAY_BASE::dynarray_base;
		explicit shared_array(size_t size) : dynarray_base{ std::make_shared_for_overwrite<T[]>(size) } {}
	};
}

#endif