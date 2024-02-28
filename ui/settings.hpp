#ifndef JUV_UI_SETTINGS
#define JUV_UI_SETTINGS

#include "../observable.hpp"

namespace juv
{
	enum struct theme
	{
		light,
		dark
	};

	struct settings
	{
		settings() = delete;
		static theme app_theme();
		static theme system_theme();
		static std::optional<theme> app_theme_override();
		static void app_theme_override(std::optional<theme>);
	};
}

#endif