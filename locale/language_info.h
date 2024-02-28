#ifndef JUV_LOCALE_LANGUAGE_INFO
#define JUV_LOCALE_LANGUAGE_INFO

#include "../immutable_string.h"

JUV_OPAQUE_STRUCT(juv_language_info, const*);


juv_language_info juv_fetch_language_info(juv_immutable_uXstring const* tag, juv_immutable_uXstring* error_message);
void juv_retain_language_info_ref(juv_language_info li);
void juv_relinquish_language_info_ref(juv_language_info li);
juv_immutable_uXstring juv_get_language_tag(juv_language_info li);
juv_immutable_uXstring juv_get_translated_language_name(juv_language_info li);
juv_immutable_uXstring juv_get_language_native_name(juv_language_info li);

#ifdef __cplusplus

namespace juv::locale
{
	struct language_info final : shared_ptr_base<language_info, juv_language_info>
	{
	public:
		immutable_uXstring tag() const
		{
			juv_get_language_tag(get());
		}

		immutable_uXstring translated_name() const
		{
			juv_get_translated_language_name(get());
		}

		immutable_uXstring native_name() const
		{
			juv_get_language_native_name(get());
		}
	private:
		void retain_ref() { juv_retain_language_info_ref(get()); }
		void relinquish_ref() { juv_relinquish_language_info_ref(get()); }
	};
}

#endif
#endif