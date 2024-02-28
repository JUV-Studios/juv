#ifndef JUV_MEASUREMENT
#define JUV_MEASUREMENT

namespace juv::ui
{
#ifdef _WIN32
	using dimension = float;
#endif

	struct point
	{
		// The horizontal (x) coördinate.
		dimension abscissa;

		// The vertical (y) coördinate.
		dimension ordinate;
	};

	struct size
	{
		// The horizontal length.
		dimension width;

		// The vertical length.
		dimension height;
	};
}

#endif