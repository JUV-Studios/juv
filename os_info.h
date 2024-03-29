#ifndef JUV_OS_INFO
#define JUV_OS_INFO

#include "immutable_string.h"
#include "semantic_version.h"

juv_immutable_uXstring juv_get_os_name();
juv_semantic_version juv_get_os_version();

#ifdef __cplusplus

namespace juv
{
	struct os_info
	{
		os_info() = delete;

		static immutable_uXstring name()
		{
			// TODO
			juv_get_os_name();
		}

		static auto version()
		{
			// TODO
			juv_get_os_version();
		}
	};
}

#endif
#endif