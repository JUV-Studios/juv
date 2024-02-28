#ifndef JUV_CHAR_CLASSIFICATION
#define JUV_CHAR_CLASSIFICATION

#include "basic_concepts.hpp"
#include <cctype>

namespace juv
{
	template <typename T>
	concept code_page_character = same_as_any<std::remove_cv_t<T>, char, wchar_t>;

	template <typename T>
	concept unicode_character = same_as_any<std::remove_cv_t<T>, char8_t, char16_t, char32_t>;

	template <typename T>
	concept character = code_page_character<T> || unicode_character<T>;

	template <character CharT>
	struct make_unicode { using type = CharT; };

	template <character CharT>
	using make_unicode_t = make_unicode<CharT>::type;

	template <typename CharT> requires std::is_const_v<CharT>
	struct make_unicode<CharT> { using type = std::add_const_t<typename make_unicode<std::remove_const_t<CharT>>::type>; };

	template <typename CharT> requires std::is_volatile_v<CharT>
	struct make_unicode<CharT> { using type = std::add_volatile_t<typename make_unicode<std::remove_volatile_t<CharT>>::type>; };

	template <>
	struct make_unicode<char> { using type = char8_t; };

	template <>
	struct make_unicode<wchar_t> { using type = std::conditional_t<sizeof(wchar_t) == 2, char16_t, char32_t>; static_assert(sizeof(wchar_t) == sizeof(type)); };

	template <character CharT>
	struct make_code_page { using type = CharT; };

	template <character CharT>
	using make_code_page_t = make_code_page<CharT>::type;

	template <typename CharT> requires std::is_const_v<CharT>
	struct make_code_page<CharT> { using type = std::add_const_t<typename make_code_page<std::remove_const_t<CharT>>::type>; };

	template <typename CharT> requires std::is_volatile_v<CharT>
	struct make_code_page<CharT> { using type = std::add_volatile_t<typename make_code_page<std::remove_volatile_t<CharT>>::type>; };

	template <>
	struct make_code_page<char8_t> { using type = char; };

	template <>
	struct make_code_page<make_unicode_t<wchar_t>> { using type = wchar_t; };

	inline bool is_space(char const chr) noexcept { return isspace(static_cast<unsigned char>(chr)); }

	inline bool is_space(wchar_t const chr) noexcept { return iswspace(static_cast<wint_t>(chr)); }

	bool constexpr is_space(char32_t const code_point) noexcept
	{
		// Reference: https://www.compart.com/en/unicode/category/Zs.
		return code_point == 0x20 || // Space
			code_point == 0xA0 || // No-break space
			code_point == 0x1680 || // Ogham space mark
			code_point == 0x202F || // Narrow no-break space
			code_point == 0x205F || // Medium mathematical space (MMSP)
			code_point == 0x3000 || // Ideographic space
			code_point >= 0x2000 && // En quad
			code_point <= 0x200A; // Hair space
	}
}

#endif