#pragma once

#include <stdint.h>
#include <math.h>

#include "kinc/simd/types.h"
#include "kinc/simd/float32x4.h"
#include "kinc/simd/int32x4.h"
#include "kinc/simd/uint32x4.h"
#include "kinc/simd/type_conversions.h"


typedef kinc_float32x4_t simd_fq;
typedef kinc_int32x4_t simd_iq;
typedef kinc_uint32x4_t simd_uq;

//implement these functions

//Floats
static inline simd_fq fq_load_u(float const * quad)
{
	return kinc_float32x4_intrin_load_unaligned(quad);
}
static inline void fq_store_u(float * destination, simd_fq t)
{
	kinc_float32x4_store_unaligned(destination, t);
}
static inline simd_fq fq_zeroes(void)
{
	return kinc_float32x4_load_all(0.0f);
}
static inline simd_fq fq_min(simd_fq a, simd_fq b)
{
	return kinc_float32x4_min(a, b);
}
static inline simd_fq fq_max(simd_fq a, simd_fq b)
{
	return kinc_float32x4_max(a, b);
}
static inline simd_fq fq_add(simd_fq a, simd_fq b)
{
	return kinc_float32x4_add(a, b);
}
static inline simd_fq fq_sub(simd_fq a, simd_fq b)
{
	return kinc_float32x4_sub(a, b);
}
static inline simd_fq fq_mul(simd_fq a, simd_fq b)
{
	return kinc_float32x4_mul(a, b);
}
static inline simd_fq fq_div(simd_fq a, simd_fq b)
{
	return kinc_float32x4_div(a, b);
}
static inline simd_fq fq_cmpeq(simd_fq a, simd_fq b)
{
	return kinc_float32x4_cmpeq(a, b);
}
static inline simd_fq fq_cmplt(simd_fq a, simd_fq b)
{
	return kinc_float32x4_cmplt(a, b);
}
static inline simd_fq fq_cmple(simd_fq a, simd_fq b)
{
	return kinc_float32x4_cmple(a, b);
}
static inline simd_fq fq_cmpgt(simd_fq a, simd_fq b)
{
	return kinc_float32x4_cmpgt(a, b);
}
static inline simd_fq fq_cmpge(simd_fq a, simd_fq b)
{
	return kinc_float32x4_cmpge(a, b);
}
static inline simd_fq fq_and(simd_fq a, simd_fq b)
{
	return kinc_float32x4_and(a, b);
}
static inline simd_fq fq_or(simd_fq a, simd_fq b)
{
	return kinc_float32x4_or(a, b);
}
static inline simd_fq fq_xor(simd_fq a, simd_fq b)
{
	return kinc_float32x4_xor(a, b);
}
static inline simd_fq fq_not(simd_fq t)
{
	return kinc_float32x4_not(t);
}
static inline simd_iq fq_as_iq(simd_fq t)
{
	return kinc_float32x4_cast_to_int32x4(t);
}
static inline simd_uq fq_as_uq(simd_fq t)
{
	return kinc_float32x4_cast_to_uint32x4(t);
}
static inline simd_iq fq_truncate_iq(simd_fq t)
{
	#if defined(KINC_SSE2)

		return _mm_cvttps_epi32(t);

	#elif defined(KINC_NEON)

		return vcvtq_s32_f32(t);

	#else

		float quadf[4];
		int32_t quad[4];
		kinc_float32x4_store_unaligned(&quadf[0], t);

		quad[0] = (int32_t)(quadf[0]);
		quad[1] = (int32_t)(quadf[1]);
		quad[2] = (int32_t)(quadf[2]);
		quad[3] = (int32_t)(quadf[3]);

		return kinc_int32x4_intrin_load_unaligned(&quad[0]);

	#endif
}

static inline simd_iq fq_round_iq(simd_fq t)
{
	#if defined(KINC_SSE2)

		return _mm_cvtps_epi32(t);

	#elif defined(KINC_NEON)

		return vcvtq_s32_f32(kinc_float32x4_add(t, kinc_float32x4_load_all(0.5f)));

	#else

		float quadf[4];
		int32_t quad[4];
		kinc_float32x4_store_unaligned(&quadf[0], t);

		quad[0] = (int32_t)roundf(quadf[0]);
		quad[1] = (int32_t)roundf(quadf[1]);
		quad[2] = (int32_t)roundf(quadf[2]);
		quad[3] = (int32_t)roundf(quadf[3]);

		return kinc_int32x4_intrin_load_unaligned(&quad[0]);

	#endif
}



//Signed Ints
static inline simd_iq iq_load_u(int32_t const * quad)
{
	return kinc_int32x4_intrin_load_unaligned(quad);
}
static inline void iq_store_u(int32_t * destination, simd_iq t)
{
	kinc_int32x4_store_unaligned(destination, t);
}
static inline simd_iq iq_zeroes(void)
{
	return kinc_int32x4_load_all(0);
}

static inline simd_iq iq_cmpeq(simd_iq a, simd_iq b)
{
	return kinc_int32x4_cmpeq(a, b);
}

static inline simd_iq iq_add(simd_iq a, simd_iq b)
{
	return kinc_int32x4_add(a, b);
}
static inline simd_iq iq_sub(simd_iq a, simd_iq b)
{
	return kinc_int32x4_sub(a, b);
}
static inline simd_iq iq_and(simd_iq a, simd_iq b)
{
	return kinc_int32x4_and(a, b);
}
static inline simd_iq iq_or(simd_iq a, simd_iq b)
{
	return kinc_int32x4_or(a, b);
}
static inline simd_iq iq_xor(simd_iq a, simd_iq b)
{
	return kinc_int32x4_xor(a, b);
}

static inline simd_fq iq_as_fq(simd_iq t)
{
	return kinc_int32x4_cast_to_float32x4(t);
}
static inline simd_uq iq_as_uq(simd_iq t)
{
	return kinc_int32x4_cast_to_uint32x4(t);
}

static inline simd_fq iq_convert_fq(simd_iq t)
{
	#if defined(KINC_SSE2)

		return _mm_cvtepi32_ps(t);

	#elif defined(KINC_NEON)

		return vcvtq_f32_s32(t);

	#else

		int32_t quad[4];
		float quadf[4];
		kinc_int32x4_store_unaligned(&quad[0], t);

		quadf[0] = (float)quad[0];
		quadf[1] = (float)quad[1];
		quadf[2] = (float)quad[2];
		quadf[3] = (float)quad[3];

		return kinc_float32x4_intrin_load_unaligned(&quadf[0]);

	#endif
}



//Un-signed Ints
static inline simd_uq uq_load_u(uint32_t const * quad)
{
	return kinc_uint32x4_intrin_load_unaligned(quad);
}
static inline void uq_store_u(uint32_t * destination, simd_uq t)
{
	kinc_uint32x4_store_unaligned(destination, t);
}
static inline simd_uq uq_zeroes(void)
{
	return kinc_uint32x4_load_all(0);
}

static inline simd_uq uq_cmpeq(simd_uq a, simd_uq b)
{
	return kinc_uint32x4_cmpeq(a, b);
}
static inline simd_uq uq_and(simd_uq a, simd_uq b)
{
	return kinc_uint32x4_and(a, b);
}
static inline simd_uq uq_or(simd_uq a, simd_uq b)
{
	return kinc_uint32x4_or(a, b);
}
static inline simd_uq uq_xor(simd_uq a, simd_uq b)
{
	return kinc_uint32x4_xor(a, b);
}

static inline simd_uq uq_not(simd_uq t)
{
	return kinc_uint32x4_not(t);
}

static inline simd_uq uq_lshift_23(simd_uq t)
{
	return kinc_uint32x4_shift_left(t, 23);
}
static inline simd_uq uq_rshift_23(simd_uq t)
{
	return kinc_uint32x4_shift_right(t, 23);
}
static inline simd_uq uq_lshift_29(simd_uq t)
{
	return kinc_uint32x4_shift_left(t, 29);
}

static inline simd_fq uq_as_fq(simd_uq t)
{
	return kinc_uint32x4_cast_to_float32x4(t);
}
static inline simd_iq uq_as_iq(simd_uq t)
{
	return kinc_uint32x4_cast_to_int32x4(t);
}
