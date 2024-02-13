#ifndef JUV_CBASE
#define JUV_CBASE
#include <stdint.h>
#include <stddef.h>
#include <uchar.h>
#ifdef __cplusplus
#define JUV_EXTERN_C_START extern "C" {
#define JUV_EXTERN_C_END }
#else
#include <stdbool.h>
#define JUV_EXTERN_C_START
#define JUV_EXTERN_C_END
#endif