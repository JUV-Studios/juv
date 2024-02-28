#ifndef JUV_UI_COLOURS
#define JUV_UI_COLOURS

#include <cstdint>
#include <format>
#ifdef _WIN32
#include <winrt/base.h>
#include <winrt/impl/Windows.UI.2.h>
#endif

namespace juv::ui
{
	namespace impl
	{
		struct colour_data
		{
			uint8_t alpha, red, green, blue;
		};
	}

	// Represents additive colour in terms of alpha, red, green and blue channels.
	struct colour : impl::colour_data
	{
		colour() = default;
		constexpr colour(uint8_t a, uint8_t r, uint8_t g, uint8_t b) noexcept : colour_data{ a, r, g, b } {}

#ifdef _WIN32
		using native_type = winrt::Windows::UI::Color;
		constexpr colour(native_type colour) noexcept : colour{ colour.A, colour.R, colour.G, colour.B } {}
		constexpr operator native_type() const noexcept { return { alpha, red, green, blue }; }
#endif

		constexpr colour(std::string_view hex)
		{
			auto from_chars = [&](size_t start, uint8_t& value)
				{
					auto const result = std::from_chars(hex.data() + start, hex.data() + start + 2, value, 16);
					if (result.ec != std::errc{}) throw std::system_error{ std::make_error_code(result.ec) };
				};

			if (hex.empty()) throw std::invalid_argument{ "hex" };
			else if (hex[0] == '#') hex.remove_prefix(1);

			if (hex.size() == 8)
			{
				from_chars(0, alpha);
				from_chars(2, red);
				from_chars(4, green);
				from_chars(6, blue);
			}
			else if (hex.size() == 6)
			{
				alpha = 255;
				from_chars(0, red);
				from_chars(2, green);
				from_chars(4, blue);
			}
			else
			{
				throw std::invalid_argument{ "hex" };
			}
		}

		bool constexpr operator==(colour const& c) const noexcept = default;
		colour constexpr operator~() const noexcept { return std::bit_cast<colour>(~std::bit_cast<uint32_t>(*this)); }
	};

	struct colours
	{
		// Copied from Windows::UI::Colors.

		colours() = delete;
		static colour constexpr alice_blue{ 255, 240, 248, 255 };
		static colour constexpr antique_white{ 255, 250, 235, 215 };
		static colour constexpr aqua{ 255,   0, 255, 255 };
		static colour constexpr aquamarine{ 255, 127, 255, 212 };
		static colour constexpr azure{ 255, 240, 255, 255 };
		static colour constexpr beige{ 255, 245, 245, 220 };
		static colour constexpr bisque{ 255, 255, 228, 196 };
		static colour constexpr black{ 255,   0,   0,   0 };
		static colour constexpr blanched_almond{ 255, 255, 235, 205 };
		static colour constexpr blue{ 255,   0,   0, 255 };
		static colour constexpr blue_violet{ 255, 138,  43, 226 };
		static colour constexpr brown{ 255, 165,  42,  42 };
		static colour constexpr burly_wood{ 255, 222, 184, 135 };
		static colour constexpr cadet_blue{ 255,  95, 158, 160 };
		static colour constexpr chartreuse{ 255, 127, 255,   0 };
		static colour constexpr chocolate{ 255, 210, 105,  30 };
		static colour constexpr coral{ 255, 255, 127,  80 };
		static colour constexpr cornflower_blue{ 255, 100, 149, 237 };
		static colour constexpr cornsilk{ 255, 255, 248, 220 };
		static colour constexpr crimson{ 255, 220,  20,  60 };
		static colour constexpr cyan{ 255,   0, 255, 255 };
		static colour constexpr dark_blue{ 255,   0,   0, 139 };
		static colour constexpr dark_cyan{ 255,   0, 139, 139 };
		static colour constexpr dark_goldenrod{ 255, 184, 134,  11 };
		static colour constexpr dark_grey{ 255, 169, 169, 169 };
		static colour constexpr dark_green{ 255,   0, 100,   0 };
		static colour constexpr dark_khaki{ 255, 189, 183, 107 };
		static colour constexpr dark_magenta{ 255, 139,   0, 139 };
		static colour constexpr dark_olive_green{ 255,  85, 107,  47 };
		static colour constexpr dark_orange{ 255, 255, 140,   0 };
		static colour constexpr dark_orchid{ 255, 153,  50, 204 };
		static colour constexpr dark_red{ 255, 139,   0,   0 };
		static colour constexpr dark_salmon{ 255, 233, 150, 122 };
		static colour constexpr dark_sea_green{ 255, 143, 188, 143 };
		static colour constexpr dark_slate_blue{ 255,  72,  61, 139 };
		static colour constexpr dark_slate_grey{ 255,  47,  79,  79 };
		static colour constexpr dark_turquoise{ 255,   0, 206, 209 };
		static colour constexpr dark_violet{ 255, 148,   0, 211 };
		static colour constexpr deep_pink{ 255, 255,  20, 147 };
		static colour constexpr deep_sky_blue{ 255,   0, 191, 255 };
		static colour constexpr dim_grey{ 255, 105, 105, 105 };
		static colour constexpr dodger_blue{ 255,  30, 144, 255 };
		static colour constexpr firebrick{ 255, 178,  34,  34 };
		static colour constexpr floral_white{ 255, 255, 250, 240 };
		static colour constexpr forest_green{ 255,  34, 139,  34 };
		static colour constexpr fuchsia{ 255, 255,   0, 255 };
		static colour constexpr gainsboro{ 255, 220, 220, 220 };
		static colour constexpr ghost_white{ 255, 248, 248, 255 };
		static colour constexpr gold{ 255, 255, 215,   0 };
		static colour constexpr goldenrod{ 255, 218, 165,  32 };
		static colour constexpr grey{ 255, 128, 128, 128 };
		static colour constexpr green{ 255,   0, 128,   0 };
		static colour constexpr green_yellow{ 255, 173, 255,  47 };
		static colour constexpr honeydew{ 255, 240, 255, 240 };
		static colour constexpr hot_pink{ 255, 255, 105, 180 };
		static colour constexpr indian_red{ 255, 205,  92,  92 };
		static colour constexpr indigo{ 255,  75,   0, 130 };
		static colour constexpr ivory{ 255, 255, 255, 240 };
		static colour constexpr khaki{ 255, 240, 230, 140 };
		static colour constexpr lavender{ 255, 230, 230, 250 };
		static colour constexpr lavender_blush{ 255, 255, 240, 245 };
		static colour constexpr lawn_green{ 255, 124, 252,   0 };
		static colour constexpr lemon_chiffon{ 255, 255, 250, 205 };
		static colour constexpr light_blue{ 255, 173, 216, 230 };
		static colour constexpr light_coral{ 255, 240, 128, 128 };
		static colour constexpr light_cyan{ 255, 224, 255, 255 };
		static colour constexpr light_goldenrod_yellow{ 255, 250, 250, 210 };
		static colour constexpr light_grey{ 255, 211, 211, 211 };
		static colour constexpr light_green{ 255, 144, 238, 144 };
		static colour constexpr light_pink{ 255, 255, 182, 193 };
		static colour constexpr light_salmon{ 255, 255, 160, 122 };
		static colour constexpr light_sea_green{ 255,  32, 178, 170 };
		static colour constexpr light_sky_blue{ 255, 135, 206, 250 };
		static colour constexpr light_slate_grey{ 255, 119, 136, 153 };
		static colour constexpr light_steel_blue{ 255, 176, 196, 222 };
		static colour constexpr light_yellow{ 255, 255, 255, 224 };
		static colour constexpr lime{ 255,   0, 255,   0 };
		static colour constexpr lime_green{ 255,  50, 205,  50 };
		static colour constexpr linen{ 255, 250, 240, 230 };
		static colour constexpr magenta{ 255, 255,   0, 255 };
		static colour constexpr maroon{ 255, 128,   0,   0 };
		static colour constexpr medium_aquamarine{ 255, 102, 205, 170 };
		static colour constexpr medium_blue{ 255,   0,   0, 205 };
		static colour constexpr medium_orchid{ 255, 186,  85, 211 };
		static colour constexpr medium_purple{ 255, 147, 112, 219 };
		static colour constexpr medium_sea_green{ 255,  60, 179, 113 };
		static colour constexpr medium_slate_blue{ 255, 123, 104, 238 };
		static colour constexpr medium_spring_green{ 255,   0, 250, 154 };
		static colour constexpr medium_turquoise{ 255,  72, 209, 204 };
		static colour constexpr medium_violet_red{ 255, 199,  21, 133 };
		static colour constexpr midnight_blue{ 255,  25,  25, 112 };
		static colour constexpr mint_cream{ 255, 245, 255, 250 };
		static colour constexpr misty_rose{ 255, 255, 228, 225 };
		static colour constexpr moccasin{ 255, 255, 228, 181 };
		static colour constexpr navajo_white{ 255, 255, 222, 173 };
		static colour constexpr navy{ 255,   0,   0, 128 };
		static colour constexpr old_lace{ 255, 253, 245, 230 };
		static colour constexpr olive{ 255, 128, 128,   0 };
		static colour constexpr olive_drab{ 255, 107, 142,  35 };
		static colour constexpr orange{ 255, 255, 165,   0 };
		static colour constexpr orange_red{ 255, 255, 69,    0 };
		static colour constexpr orchid{ 255, 218, 112, 214 };
		static colour constexpr pale_goldenrod{ 255, 238, 232, 170 };
		static colour constexpr pale_green{ 255, 152, 251, 152 };
		static colour constexpr pale_turquoise{ 255, 175, 238, 238 };
		static colour constexpr pale_violet_red{ 255, 219, 112, 147 };
		static colour constexpr papaya_whip{ 255, 255, 239, 213 };
		static colour constexpr peach_puff{ 255, 255, 218, 185 };
		static colour constexpr peru{ 255, 205, 133,  63 };
		static colour constexpr pink{ 255, 255, 192, 203 };
		static colour constexpr plum{ 255, 221, 160, 221 };
		static colour constexpr powder_blue{ 255, 176, 224, 230 };
		static colour constexpr purple{ 255, 128,   0, 128 };
		static colour constexpr red{ 255, 255,   0,   0 };
		static colour constexpr rosy_brown{ 255, 188, 143, 143 };
		static colour constexpr royal_blue{ 255,  65, 105, 225 };
		static colour constexpr saddle_brown{ 255, 139,  69,  19 };
		static colour constexpr salmon{ 255, 250, 128, 114 };
		static colour constexpr sandy_brown{ 255, 244, 164,  96 };
		static colour constexpr sea_green{ 255,  46, 139,  87 };
		static colour constexpr sea_shell{ 255, 255, 245, 238 };
		static colour constexpr sienna{ 255, 160,  82,  45 };
		static colour constexpr silver{ 255, 192, 192, 192 };
		static colour constexpr sky_blue{ 255, 135, 206, 235 };
		static colour constexpr slate_blue{ 255, 106,  90, 205 };
		static colour constexpr slate_grey{ 255, 112, 128, 144 };
		static colour constexpr snow{ 255, 255, 250, 250 };
		static colour constexpr spring_green{ 255,   0, 255, 127 };
		static colour constexpr steel_blue{ 255,  70, 130, 180 };
		static colour constexpr tan{ 255, 210, 180, 140 };
		static colour constexpr teal{ 255,   0, 128, 128 };
		static colour constexpr thistle{ 255, 216, 191, 216 };
		static colour constexpr tomato{ 255, 255,  99,  71 };
		static colour constexpr transparent{ 0, 255, 255, 255 };
		static colour constexpr turquoise{ 255,  64, 224, 208 };
		static colour constexpr violet{ 255, 238, 130, 238 };
		static colour constexpr wheat{ 255, 245, 222, 179 };
		static colour constexpr white{ 255, 255, 255, 255 };
		static colour constexpr whiteSmoke{ 255, 245, 245, 245 };
		static colour constexpr yellow{ 255, 255, 255,   0 };
		static colour constexpr yellow_green{ 255, 154, 205,  50 };
	};
}

template <>
struct std::formatter<juv::ui::colour>
{
	// TODO Add option for lowercase and ignoring alpha.
	auto constexpr parse(std::format_parse_context& context)
	{
		return context.begin();
	}

	/*auto constexpr parse(std::wformat_parse_context& context)
	{
		return context.begin();
	}*/

	// TODO use target type string literal
	/*auto format(juv::ui::colour const& colour, std::wformat_context& context) const
	{
		return std::format_to(context.out(), L"#{:2X}{:2X}{:2X}{:2X}", colour.alpha, colour.red, colour.green, colour.blue);
	}*/

	auto format(juv::ui::colour const& colour, std::format_context& context) const
	{
		return std::format_to(context.out(), "#{:2X}{:2X}{:2X}{:2X}", colour.alpha, colour.red, colour.green, colour.blue);
	}
};

#endif