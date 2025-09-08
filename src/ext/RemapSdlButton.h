/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#pragma once

#include "ext.h"

EXTERN_C int RemapSdlButton(int button);
EXTERN_C void ChangeSdlButtonMapping(int sdlButton, int internalButton);

EXTERN_C void saveButtonConfig();
EXTERN_C void loadButtonConfig();
