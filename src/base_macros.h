#ifndef BASE_MACROS_H
#define BASE_MACROS_H

#include <stdio.h>
#if !defined(ASSERT_BREAK)
#   define ASSERT_BREAK(_x) do { fprintf(stderr, "assertion `%s` failed [%s:%u]\n", #_x, __FILE__, __LINE__); *(int*)0=0; } while(0) // crash (ouch)
#endif

#if ENABLE_ASSERT
#   define ASSERT(_x) do { if(!(_x)) ASSERT_BREAK((_x)); } while(0)
#else
#   define ASSERT(_x)
#endif

#define ARRAY_SIZE(_arr) (sizeof((_arr))/(sizeof(*(_arr))))

#define KILOBYTES(_x) (1024*(_x))
#define MEGABYTES(_x) (1024*KILOBYTES((_x)))
#define GIGABYTES(_x) (1024*MEGABYTES((_x)))

// NOTE(nix3l): might be useful for ecs later on
#define INT_FROM_PTR(_ptr) (unsigned long long)((char*)_ptr-(char*)0)
#define PTR_FROM_INT(_int) (void*)((char*)0 + (_int))

// very interesting, gives the abstract representation of a member of a struct so to speak
#define MEMBER(_T, _m) (((_T*)0)->_m)
// NOTE(nix3l): might be useful for serialisation later on probably
#define OFFSET_TO_MEMBER(_T, _m) INT_FROM_PTR(&MEMBER(_T, _m))

// macros here so we dont have to have separate functions for each data type
// so can be used with floats, doubles, all types of ints, etc
#define MIN(_a, _b) ((_a)<(_b)?(_a):(_b))
#define MAX(_a, _b) ((_a)>(_b)?(_a):(_b))
#define CLAMP(_x, _min, _max) ((_x)<(_min)?(_min):((_x)>(_max)?(_max):(_x)))
#define CLAMP_MAX(_x, _max) MIN(_x, _max)
#define CLAMP_MIN(_x, _min) MAX(_x, _min)

#include <string.h>
#define MEM_ZERO(_ptr, _num) memset((_ptr), 0, (_num))
#define MEM_ZERO_STRUCT(_ptr) MEM_ZERO(_ptr, sizeof(*(_ptr)))
#define MEM_ZERO_ARRAY(_ptr) MEM_ZERO(_ptr, sizeof((_ptr)))

#define VECTOR_2(_x, _y) (v2f) { .x = (_x), .y = (_y) }
#define VECTOR_3(_x, _y, _z) (v3f) { .x = (_x), .y = (_y), .z = (_z) }
#define VECTOR_4(_x, _y, _z, _w) (v4f) { .x = (_x), .y = (_y), .z = (_z), .w = (_w) }

#define VECTOR_2_ZERO() VECTOR_2(0.0f, 0.0f)
#define VECTOR_3_ZERO() VECTOR_3(0.0f, 0.0f, 0.0f)
#define VECTOR_4_ZERO() VECTOR_4(0.0f, 0.0f, 0.0f, 0.0f)

#define VECTOR_2_ONE() VECTOR_2(1.0f, 1.0f)
#define VECTOR_3_ONE() VECTOR_3(1.0f, 1.0f, 1.0f)
#define VECTOR_4_ONE() VECTOR_4(1.0f, 1.0f, 1.0f, 1.0f)

#define MAT3_IDENTITY (glms_mat3_identity())
#define MAT4_IDENTITY (glms_mat4_identity())

#define RADIANS(_x) (glm_rad((_x)))
#define DEGREES(_x) (glm_deg((_x)))

#define RAND_IN_RANGE(_min, _max) ((_min) + ((float) rand() / (float) RAND_MAX) * ((_max) - (_min)))

#endif
