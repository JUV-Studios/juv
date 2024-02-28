#ifndef JUV_SEMANTIC_VERSION
#define JUV_SEMANTIC_VERSION

#include "cbase.h"

typedef struct
{
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t prerelease_type; // 0: alpha, 1: beta, 2: release candidate, 3 or other: none
	uint16_t prerelease_number;
} juv_semantic_version;

#ifdef __cplusplus
#include <neargye/semver.hpp>

namespace juv
{
	auto constexpr wrap_semantic_version(juv_semantic_version sv) noexcept
	{
		// TODO
	}
}

#endif
#endif