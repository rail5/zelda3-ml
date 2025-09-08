/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#pragma once

/**
 * These extensions under src/ext are written in C++, but the rest of the codebase is C.
 * Therefore they include many functions that need to be called from C code.
 *
 * To ensure proper linkage, we use the EXTERN_C macro to wrap these function declarations.
 *
 * We will only declare these functions to have C external linkage **if** we're in a C++ compiler.
 * If we're in a C compiler, then obviously they already have C linkage.
 */
#ifdef __cplusplus
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C
#endif
