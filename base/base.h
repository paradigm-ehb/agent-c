#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stddef.h>

#define global_variable static
#define local_persist   static
#define local_internal static

#define ERR_OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

#define KiB(n) (((u64)(n)) << 10)
#define MiB(n) (((u64)(n)) << 20)
#define GiB(n) (((u64)(n)) << 30)

#define BUFFER_SIZE_SMALL 128
#define BUFFER_SIZE_DEFAULT 256
#define BUFFER_SIZE_LARGE 512
#define PATH_MAX_LEN 4096


#define DEPRECATED __attribute__((__deprecated__))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef i32 b32;
typedef i16 b16;
typedef u8 b8;

#define TRUE 1
#define FALSE 0

local_internal u64
parse_u64(char *buf, size_t len);

local_internal b8
is_numeric(char *s);

local_internal b8
compare_string(const char *c1, const char *c2);

#endif
