#ifndef JUV_IMMUTABLE_STRING
#define JUV_IMMUTABLE_STRING
#include "cbase.h"

typedef struct
{
	void (*add_ref)(void* self);
	void (*release_ref)(void* self);
	bool (*identify_type)(void* self, void const* type_id);
} juv_immutable_string_vtable;

#define JUV_IMPL_IMMUTABLE_STRING_FIELDS(type) juv_immutable_string_vtable vtable; type const* data; size_t size; void* self;

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
} juv_immutable_uxstring;

#ifdef __cplusplus
#include "wrapperers.hpp"

namespace juv
{
	template <string_unit CharT>
	struct basic_immutable_string : string_view_base<CharT, size_t>
	{
	};

	using immutable_string = basic_immutable_string<char>;
	using immutable_wstring = basic_immutable_string<wchar_t>;
	using immutable_u8string = basic_immutable_string<char8_t>;
	using immutable_u16string = basic_immutable_string<char16_t>;
	using immutable_u32string = basic_immutable_string<char32_t>;
	using immutable_uxstring = basic_immutable_string<void>;
}

#endif
#endif