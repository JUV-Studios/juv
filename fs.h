#ifndef JUV_FS
#define JUV_FS

#include "async.h"
#include "absolute_uri.h"
#include <stdio.h>

typedef struct
{
	size_t version;
	void (*retain_ref)(void* self);
	void (*relinquish_ref)(void* self);
	juv_async_task (*get_contained_folder)(void* self);
	juv_immutable_uXstring (*get_id)(void* self);
	juv_immutable_uXstring(*get_display_name)(void* self);
	juv_absolute_uri (*get_location)(void* self);
} juv_fs_storable_vtable;

typedef struct
{
	juv_fs_storable_vtable const* storable_vtable;
} *juv_fs_storable;

typedef struct
{
	FILE* (*open)(void* self, char const* mode);
	juv_async_task (*open_async)(void* self);
} juv_fs_file_vtable;

typedef struct
{
	juv_fs_storable_vtable const* storable_vtable;
	juv_fs_file_vtable const* file_vtable;
} *juv_fs_file;

typedef struct
{
	juv_async_task (*create_item_async)(void* self, juv_immutable_uXstring const* name, uint8_t kind);
} juv_fs_folder_vtable;

typedef struct
{
	juv_fs_storable_vtable const* storable_vtable;
	juv_fs_folder_vtable const* folder_vtable;
} *juv_fs_folder;

typedef struct
{
} juv_fs_native_storable;

typedef struct
{
} juv_fs_native_file;

typedef struct
{
} juv_fs_native_folder;

#ifdef _WIN32
#include <Inspectable.h>
HRESULT juv_fs_storable_from_winrt(IInspectable* /* of any interface */ storage_item, juv_fs_storable* out);
#endif
#ifdef __cplusplus
#ifdef _WIN32
#include <winrt/Windows.Storage.h> 
#endif

namespace juv::fs
{
	struct storable final
	{
#ifdef _WIN32
		storable(std::convertible_to<winrt::Windows::Storage::IStorageItem> auto&& storage_item);
#endif
	};

	struct file final
	{
#ifdef _WIN32
		file(std::convertible_to<winrt::Windows::Storage::IStorageFile> auto&& storage_item);
#endif
	};

	struct folder final
	{
#ifdef _WIN32
		folder(std::convertible_to<winrt::Windows::Storage::IStorageFile> auto&& storage_item);
#endif
	};

	struct native_file final
	{
	};

	struct native_folder final
	{
	};
}

#endif
#endif