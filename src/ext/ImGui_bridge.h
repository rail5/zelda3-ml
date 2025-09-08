/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#pragma once

#include "ext.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

extern SDL_GLContext gl_context;

EXTERN_C void ImGui_BeginFrame(SDL_Window* window);
EXTERN_C void ImGui_EndFrame();
EXTERN_C void ImGui_ShowToolbar();
EXTERN_C void ImGui_ProcessEvent(const SDL_Event* event);
