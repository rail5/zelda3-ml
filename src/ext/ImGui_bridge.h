/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_GLContext gl_context;

void ImGui_BeginFrame(SDL_Window* window);
void ImGui_EndFrame();
void ImGui_ShowToolbar();
void ImGui_ProcessEvent(const SDL_Event* event);

bool ImGui_WantCaptureMouse();
bool ImGui_WantCaptureKeyboard();

extern bool remapping_active;
extern int remapping_internal_button;

#ifdef __cplusplus
}
#endif
