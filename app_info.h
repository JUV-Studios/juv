#ifndef JUV_APP_INFO
#define JUV_APP_INFO
#include "immutable_string.h"
#include "semantic_version.h"

JUV_EXTERN_C_START
typedef struct juv_app_info
{
	void* self;
	struct
	{
		size_t version;
		void (*add_ref)(void* self);
		void (*release_ref)(void* self);
		juv_immutable_u8string (*get_display_name)(void* self);
		juv_immutable_u8string (*get_description)(void* self);
		juv_immutable_u8string (*get_publisher_name)(void* self);
		juv_semantic_version (*get_version)(void* self);
	} const* vtable;
};

juv_app_info juv_get_current_app_info();
JUV_EXTERN_C_END

#ifdef __cplusplus
namespace juv
{
	struct app_info : shared_ptr_base<app_info, juv_app_info>
	{
	public:
		using shared_ptr_base::shared_ptr_base;

		app_info() : app_info{ juv_get_current_app_info(), take_ownership_from_abi } {}

		auto display_name() const
		{
			// TODO
			get().vtable->get_name(get().vtable);
		}

		auto description() const
		{
			get().vtable->get_description(get().vtable);
		}

		auto publisher_name() const
		{
			get().vtable->get_publisher_name(get());
		}

		auto version() const
		{
			return juv::wrap_semantic_version(get().vtable->get_version(get()));
		}
	private:
		void add_ref()
		{
			get().vtable->add_ref(get().self);
		}

		void release_ref()
		{
			get().vtable->release_ref(get().self);
		}
	};
}
#endif
#endif