#pragma once


//README:
//	Do not include this file directly
//	This is intended to be the interface that you implement in a separate file
//	Edit SIMD_trig and import your implementation into it.

/*

// #include <stdint.h>

//Define your types as:
//	float32x4 ----> simd_fq
//	int32x4 ------> simd_sq
//	uint32x4 -----> simd_uq

//Defaults, redefine these to your underlying SIMD type
typedef struct simd_fq { float values[4]; } simd_fq;
typedef struct simd_iq { int32_t values[4]; } simd_iq;
typedef struct simd_uq { uint32_t values[4]; } simd_uq;

//implement these functions

//Floats
static inline simd_fq fq_load_u(float const * quad);
static inline void fq_store_u(float * destination, simd_fq t);
static inline simd_fq fq_zeroes(void);

static inline simd_fq fq_min(simd_fq a, simd_fq b);
static inline simd_fq fq_max(simd_fq a, simd_fq b);

static inline simd_fq fq_cmpeq(simd_fq a, simd_fq b);
static inline simd_fq fq_cmplt(simd_fq a, simd_fq b);
static inline simd_fq fq_cmple(simd_fq a, simd_fq b);
static inline simd_fq fq_cmpgt(simd_fq a, simd_fq b);
static inline simd_fq fq_cmpge(simd_fq a, simd_fq b);

static inline simd_fq fq_add(simd_fq a, simd_fq b);
static inline simd_fq fq_sub(simd_fq a, simd_fq b);
static inline simd_fq fq_mul(simd_fq a, simd_fq b);
static inline simd_fq fq_div(simd_fq a, simd_fq b);
static inline simd_fq fq_and(simd_fq a, simd_fq b);
static inline simd_fq fq_or(simd_fq a, simd_fq b);
static inline simd_fq fq_xor(simd_fq a, simd_fq b);
static inline simd_fq fq_not(simd_fq t);

static inline simd_iq fq_as_iq(simd_fq t);
static inline simd_uq fq_as_uq(simd_fq t);

static inline simd_iq fq_truncate_iq(simd_fq t);
static inline simd_iq fq_round_iq(simd_fq t);



//Signed Ints
static inline simd_iq iq_load_u(int32_t const * quad);
static inline void iq_store_u(int32_t * destination, simd_iq t);
static inline simd_iq iq_zeroes(void);

static inline simd_iq iq_add(simd_iq a, simd_iq b);
static inline simd_iq iq_sub(simd_iq a, simd_iq b);
static inline simd_iq iq_mul(simd_iq a, simd_iq b);
static inline simd_iq iq_and(simd_iq a, simd_iq b);
static inline simd_iq iq_or(simd_iq a, simd_iq b);
static inline simd_iq iq_xor(simd_iq a, simd_iq b);

static inline simd_fq iq_as_fq(simd_iq t);
static inline simd_uq iq_as_uq(simd_iq t);

static inline simd_fq iq_convert_fq(simd_iq t);



//Un-signed Ints
static inline simd_uq uq_load_u(uint32_t const * quad);
static inline void uq_store_u(uint32_t * destination, simd_uq t);
static inline simd_uq uq_zeroes(void);

static inline simd_uq uq_cmpeq(simd_uq a, simd_uq b);
static inline simd_uq uq_and(simd_uq a, simd_uq b);
static inline simd_uq uq_or(simd_uq a, simd_uq b);
static inline simd_uq uq_xor(simd_uq a, simd_uq b);

static inline simd_uq uq_lshift_23(simd_uq t);
static inline simd_uq uq_lshift_29(simd_uq t);
static inline simd_uq uq_rshift_23(simd_uq t);

static inline simd_fq uq_as_fq(simd_uq t);
static inline simd_iq uq_as_iq(simd_uq t);




*/