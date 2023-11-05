#pragma once

//Always include C's extended scalar types
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef _MSC_VER
	#define RL_INLINE static __forceinline
#else
	#define RL_INLINE static __attribute__((always_inline))
#endif

#ifndef NDEBUG
	#if defined(_MSC_VER)
		#define RL_ASSERT(CONDITION) if (!(CONDITION)) { __debugbreak(); }
	#elif defined(__clang__)
		#define RL_ASSERT(CONDITION) if (!(CONDITION)) { __builtin_debugtrap(); }
	#else
		#if defined(__aarch64__)
			#define RL_ASSERT(CONDITION) if (!(CONDITION)) { __asm__ volatile(".inst 0xd4200000"); }
		#elif defined(__x86_64__)
			#define RL_ASSERT(CONDITION) if (!(CONDITION)) { __asm__ volatile("int $0x03"); }
		#endif
	#endif
#endif

#define RL_NO_OPERATION() ((void)0)
#define RL_CAST(TARGET_TYPE, SOURCE_TYPE) (TARGET_TYPE)(SOURCE_TYPE)