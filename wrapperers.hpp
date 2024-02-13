#ifndef JUV_WRAPPERERS
#define JUV_WRAPPERERS

#include "char_classification.hpp"
#include <assert.hpp>
#include <stdexcept>
#if __has_include(<winrt/base.h>)
#include <winrt/base.h>
#endif

namespace juv
{
#ifdef WINRT_BASE_H
	using winrt::take_ownership_from_abi_t;
	using winrt::take_ownership_from_abi;
#else
	struct take_ownership_from_abi_t {};
	inline constexpr take_ownership_from_abi_t take_ownership_from_abi{};
#endif

	// TODO add to C++/WinRT - https://github.com/microsoft/cppwinrt/issues/1382.
	struct duplicate_from_abi_t {};
	inline constexpr duplicate_from_abi_t duplicate_from_abi{};

	namespace impl
	{
		template <typename Derived, typename Pointer>
		struct nullable_ptr_base
		{
		public:
			using underlying_type = Pointer;

			nullable_ptr_base(std::nullptr_t) noexcept { do_make_null(); }
			nullable_ptr_base(nullable_ptr_base&& other) noexcept : m_underlying{ other.detach() } {}
			nullable_ptr_base(Pointer underlying, take_ownership_from_abi_t) noexcept : m_underlying{ underlying } {}

			explicit operator bool() const noexcept
			{
				auto& derived = JUV_DERIVED;
				if constexpr (requires { derived.is_null(); }) return derived.is_null();
				else if constexpr (nullable_pointer<Pointer>) return static_cast<bool>(m_underlying);
				else static_assert("No known way to test for nullability; please implement the is_null function.");
			}

			[[nodiscard]] bool operator==(std::nullptr_t) const noexcept { return !operator bool(); }

			auto const& get() const noexcept { return m_underlying; }

			auto detach() noexcept
			{
				auto const underlying = m_underlying;
				do_make_null();
				return underlying;
			}
		protected:
			Pointer m_underlying;
		private:

			void do_make_null() noexcept
			{
				auto& derived = JUV_DERIVED;
				if constexpr (requires { derived.make_null(); }) derived.make_null();
				else if constexpr (nullable_pointer<Pointer>) m_underlying = nullptr;
				else static_assert("No known way to null out the pointer; please implement the make_null function.");
			}
		};
	}

	#define JUV_IMPL_NULLABLE_PTR_AS_BASE impl::nullable_ptr_base<Derived, Pointer>

	template <typename Derived, typename Pointer>
	struct unique_ptr_base : JUV_IMPL_NULLABLE_PTR_AS_BASE
	{
		using JUV_IMPL_NULLABLE_PTR_AS_BASE::nullable_ptr_base;

		~unique_ptr_base()
		{
			JUV_DERIVED.destroy();
		}

		auto& operator=(unique_ptr_base other) noexcept
		{
			JUV_DERIVED.destroy();
			this->m_underlying = other.detach();
			return *this;
		}
	};

	template <typename Derived, typename Pointer>
	struct shared_ptr_base : JUV_IMPL_NULLABLE_PTR_AS_BASE
	{		
		using JUV_IMPL_NULLABLE_PTR_AS_BASE::nullable_ptr_base;

		shared_ptr_base(shared_ptr_base&&) = default;
		shared_ptr_base(shared_ptr_base const& other) noexcept : shared_ptr_base{ other.m_underlying, duplicate_from_abi } {}
		shared_ptr_base(Pointer underlying, duplicate_from_abi_t) noexcept : impl::nullable_ptr_base{ underlying, take_ownership_from_abi }
		{
			JUV_DERIVED.add_ref();
		}

		~shared_ptr_base()
		{
			JUV_DERIVED.release_ref();
		}

		auto& operator=(shared_ptr_base<Derived, Pointer> other) noexcept
		{
			JUV_DERIVED.release_ref();
			this->m_underlying = other.detach();
			return *this;
		}
	};

	namespace impl
	{
		template <typename Derived, typename SizeT, typename ViewT>
		struct container_view_base
		{
			static auto constexpr out_of_range_message = "Container subscript was out of range.";

			constexpr operator ViewT() const
			{
				return { JUV_DERIVED.data(), JUV_DERIVED.size() };
			}

			[[nodiscard]] bool constexpr empty() const { return JUV_DERIVED.size() == 0; }

			decltype(auto) operator[](SizeT pos)
			{
				DEBUG_ASSERT(pos >= JUV_DERIVED.size(), out_of_range_message);
				return JUV_DERIVED.data()[pos];
			}

			decltype(auto) operator[](SizeT pos) const
			{
				DEBUG_ASSERT(pos >= JUV_DERIVED.size(), out_of_range_message);
				return JUV_DERIVED.data()[pos];
			}

			decltype(auto) at(SizeT pos)
			{
				if (pos >= JUV_DERIVED.size()) throw std::out_of_range{ out_of_range_message };
				return JUV_DERIVED.data()[pos];
			}

			decltype(auto) at(SizeT pos) const
			{
				if (pos >= JUV_DERIVED.size()) throw std::out_of_range{ out_of_range_message };
				return JUV_DERIVED.data()[pos];
			}

			auto constexpr front() { return operator[](0); }
			auto constexpr front() const { return operator[](0); }

			auto constexpr back() { return operator[][JUV_DERIVED.size() - 1]; }
			auto constexpr back() const { return operator[](JUV_DERIVED.size() - 1); }

			auto constexpr begin() { return JUV_DERIVED.data(); }
			auto constexpr begin() const { return JUV_DERIVED.data(); }
			auto constexpr cbegin() const { return begin(); }

			auto constexpr rbegin() { return std::reverse_iterator{ end() }; }
			auto constexpr rbegin() const { return std::reverse_iterator{ end() }; }
			auto constexpr crbegin() const { return rbegin(); }

			auto constexpr end() { return begin() + JUV_DERIVED.size(); }
			auto constexpr end() const { return begin() + JUV_DERIVED.size(); }
			auto constexpr cend() const { return end(); }

			auto constexpr rend() { return std::reverse_iterator{ begin() }; }
			auto constexpr rend() const { return std::reverse_iterator{ begin() }; }
			auto constexpr crend() const { return rend(); }
		};
	}

	template <typename Derived, string_unit CharT, std::unsigned_integral SizeT, typename Traits = std::char_traits<CharT>, typename ViewT = std::basic_string_view<CharT, Traits>>
	struct string_view_base : impl::container_view_base<Derived, SizeT, ViewT>
	{
	private:
		static auto to_view_size(SizeT value) noexcept
		{
			if constexpr (!std::same_as<SizeT, ViewT::size_type>)
			{
				if (value == -1) return ViewT::size_type{ -1 };
			}

			return value;
		}

		auto to_our_size(ViewT::size_type value) noexcept
		{
			if constexpr (!std::same_as<SizeT, ViewT::size_type>)
			{
				if (value == -1) return SizeT{ -1 };
			}

			return value;
		}
	public:
		using view_type = ViewT;
		using traits_type = Traits;
		using value_type = CharT;
		using size_type = SizeT;
		using difference_type = std::make_signed_t<SizeT>;
		using pointer = ViewT::pointer;
		using const_pointer = ViewT::const_pointer;
		using reference = ViewT::reference;
		using const_reference = ViewT::const_reference;
		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static constexpr size_type npos = -1;

		size_type constexpr length() const { return JUV_DERIVED.size(); }

		size_type constexpr copy(pointer dest, size_type count, size_type pos = 0) const { return to_our_size(view_type{ *this }.copy(dest, count, to_view_size(pos))); }

		view_type constexpr substr(size_type pos, size_type count = npos) const { return view_type{ *this }.substr(pos, to_view_size(count)); }

		int constexpr compare(view_type v) const { return view_type{ *this }.compare(v); }
		int constexpr compare(CharT const* s) const { return view_type{ *this }.compare(s); }
		int constexpr compare(size_type pos1, size_type count1, view_type v) const { return view_type{ *this }.compare(to_view_size(pos1), to_view_size(count1), v); }
		int constexpr compare(size_type pos1, size_type count1, CharT const* s) const { return view_type{ *this }.compare(to_view_size(pos1), to_view_size(count1), s); }
		int constexpr compare(size_type pos1, size_type count1, CharT const* s, size_type count2) const { return view_type{ *this }.compare(to_view_size(pos1), to_view_size(count1), s, to_view_size(count2)); }
		int constexpr compare(size_type pos1, size_type count1, view_type v, size_type pos2, size_type count2) const { return view_type{ *this }.compare(to_view_size(pos1), to_view_size(count1), v, to_view_size(pos2), to_view_size(count2)); }

		bool constexpr starts_with(view_type sv) const { return view_type{ *this }.starts_with(sv); }
		bool constexpr starts_with(CharT ch) const { return view_type{ *this }.starts_with(ch); }
		bool constexpr starts_with(CharT const* s) const { return view_type{ *this }.starts_with(s); }

		bool constexpr ends_with(view_type sv) const { return view_type{ *this }.ends_with(sv); }
		bool constexpr ends_with(CharT ch) const { return view_type{ *this }.ends_with(ch); }
		bool constexpr ends_with(CharT const* s) const { return view_type{ *this }.ends_with(s); }

#ifdef __cpp_lib_string_contains
		bool constexpr contains(view_type sv) const { return view_type{ *this }.contains(sv); }
		bool constexpr contains(CharT ch) const { return view_type{ *this }.contains(ch); }
		bool constexpr contains(CharT const* s) const { return view_type{ *this }.contains(s); }
#endif

		size_type constexpr find(view_type v, size_type pos = 0) const { return to_our_size(view_type{ *this }.find(v, to_view_size(pos))); }
		size_type constexpr find(CharT ch, size_type pos = 0) const { return to_our_size(view_type{ *this }.find(ch, to_view_size(pos))); }
		size_type constexpr find(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.find(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr find(CharT const* s, size_type pos = 0) const { return to_our_size(view_type{ *this }.find(s, to_view_size(pos))); }

		size_type constexpr rfind(view_type v, size_type pos = npos) const { return to_our_size(view_type{ *this }.rfind(to_view_size(pos))); }
		size_type constexpr rfind(CharT ch, size_type pos = npos) const { return to_our_size(view_type{ *this }.rfind(to_view_size(pos))); }
		size_type constexpr rfind(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.rfind(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr rfind(CharT const* s, size_type pos = npos) const { return to_our_size(view_type{ *this }.rfind(s, to_view_size(pos))); }

		size_type constexpr find_first_of(view_type v, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_of(v, to_view_size(pos))); }
		size_type constexpr find_first_of(CharT ch, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_of(ch, to_view_size(pos))); }
		size_type constexpr find_first_of(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.find_first_of(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr find_first_of(CharT const* s, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_not_of(s, to_view_size(pos))); }

		size_type constexpr find_last_of(view_type v, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_of(v, to_view_size(pos))); }
		size_type constexpr find_last_of(CharT ch, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_of(ch, to_view_size(pos))); }
		size_type constexpr find_last_of(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.find_last_of(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr find_last_of(CharT const* s, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_of(s, to_view_size(pos))); }

		size_type constexpr find_first_not_of(view_type v, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_not_of(v, to_view_size(pos))); }
		size_type constexpr find_first_not_of(CharT ch, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_not_of(ch, to_view_size(pos))); }
		size_type constexpr find_first_not_of(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.find_first_not_of(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr find_first_not_of(CharT const* s, size_type pos = 0) const { return to_our_size(view_type{ *this }.find_first_not_of(s, to_view_size(pos))); }

		size_type constexpr find_last_not_of(view_type v, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_not_of(v, to_view_size(pos))); }
		size_type constexpr find_last_not_of(CharT ch, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_not_of(ch, to_view_size(pos))); }
		size_type constexpr find_last_not_of(CharT const* s, size_type pos, size_type count) const { return to_our_size(view_type{ *this }.find_last_not_of(s, to_view_size(pos), to_view_size(count))); }
		size_type constexpr find_last_not_of(CharT const* s, size_type pos = npos) const { return to_our_size(view_type{ *this }.find_last_not_of(s, to_view_size(pos))); }
	};

	template <typename Derived, typename ValueT, std::unsigned_integral SizeT, typename ViewT = std::span<ValueT>>
	struct array_view_base : impl::container_view_base<Derived, SizeT, ViewT>
	{
		using view_type = ViewT;
	};
}

#endif