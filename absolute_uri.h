#ifndef JUV_ABSOLUTE_URI
#define JUV_ABSOLUTE_URI

#include "immutable_string.h"

JUV_OPAQUE_STRUCT(juv_absolute_uri, const*);

void juv_retain_uri_ref(juv_absolute_uri);
void juv_relinquish_uri_ref(juv_absolute_uri);
juv_absolute_uri juv_parse_uri(juv_immutable_uXstring const*, juv_immutable_uXstring* error_message);

#ifdef __cplusplus

namespace juv
{
	struct absolute_uri : shared_ptr_base<absolute_uri, juv_absolute_uri>
	{
		absolute_uri(immutable_uXstring);
	};
}

#endif
#endif