#ifndef JUV_IMMUTABLE_STRING
#define JUV_IMMUTABLE_STRING

#include "cbase.h"

typedef struct
{
	void (*retain_ref)(void* self);
	void (*relinquish_ref)(void* self);
	bool (*identify_type)(void* self, void const* type_id);
} juv_immutable_string_vtable;

#define JUV_IMPL_IMMUTABLE_STRING_FIELDS(type) juv_immutable_string_vtable* vtable; type const* data; size_t size; void* self;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(char);
} juv_immutable_string;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(wchar_t);
} juv_immutable_wstring;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(char8_t);
} juv_immutable_u8string;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(char16_t);
} juv_immutable_u16string;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(char32_t);
} juv_immutable_u32string;

typedef struct
{
	JUV_IMPL_IMMUTABLE_STRING_FIELDS(void);
	uint8_t Encoding;
} juv_immutable_uXstring;

#ifdef __cplusplus
#include "wrapperers.hpp"

namespace juv
{
	namespace impl
	{
		template <typename T>
		struct immutable_string_underlying;

		template <>
		struct immutable_string_underlying<char> { using type = juv_immutable_string; };

		template <>
		struct immutable_string_underlying<wchar_t> { using type = juv_immutable_wstring; };

		template <>
		struct immutable_string_underlying<char8_t> { using type = juv_immutable_u8string; };

		template <>
		struct immutable_string_underlying<char16_t> { using type = juv_immutable_u16string; };

		template <>
		struct immutable_string_underlying<char32_t> { using type = juv_immutable_u32string; };

		template <>
		struct immutable_string_underlying<void> { using type = juv_immutable_uXstring; };

		template <typename Derived, typename CharT>
		struct immutable_string_wrapper_base : string_view_base<Derived, CharT, size_t>, shared_ptr_base<Derived, typename impl::immutable_string_underlying<CharT>::type>
		{
			auto data() const noexcept { return this->get().data; }
			auto c_str() const noexcept { return this->get().data; }
			auto size() const noexcept { return this->get().size; }
			bool is_everlasting() const noexcept { return this->get().vtable->relinquish_ref != nullptr; }
		};
	}

	template <typename T>
	struct basic_immutable_string;

	template <character CharT>
	struct basic_immutable_string<CharT> : impl::immutable_string_wrapper_base<basic_immutable_string<CharT>, CharT>
	{
	};

	template <>
	struct basic_immutable_string<void>
	{
	};

	using immutable_string = basic_immutable_string<char>;
	using immutable_wstring = basic_immutable_string<wchar_t>;
	using immutable_u8string = basic_immutable_string<char8_t>;
	using immutable_u16string = basic_immutable_string<char16_t>;
	using immutable_u32string = basic_immutable_string<char32_t>;
	using immutable_uXstring = basic_immutable_string<void>;

	template <character CharT>
	struct basic_immutable_string_view : impl::immutable_string_wrapper_base<basic_immutable_string<CharT>, CharT>
	{
		bool is_owning() const noexcept { return this->get().vtable != nullptr; }
	};

	using immutable_string_view = basic_immutable_string_view<char>;
	using immutable_wstring_view = basic_immutable_string_view<wchar_t>;
	using immutable_u8string_view = basic_immutable_string_view<char8_t>;
	using immutable_u16string_view = basic_immutable_string_view<char16_t>;
	using immutable_u32string_view = basic_immutable_string_view<char32_t>;
	//using immutable_uXstring_view = basic_immutable_string_view<void>;
}

#endif
#endif