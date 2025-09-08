/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#pragma once

/**
 * Only declare these functions to have C external linkage **if** we're in a C++ compiler.
 * If we're in a C compiler, then obviously they already have C linkage.
 */
#ifdef __cplusplus
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C
#endif

EXTERN_C int RemapSdlButton(int button);
EXTERN_C void ChangeSdlButtonMapping(int sdlButton, int internalButton);

EXTERN_C void saveButtonConfig();
EXTERN_C void loadButtonConfig();
