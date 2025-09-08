/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#include <SDL2/SDL_video.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGui_bridge.h"

static ImGuiContext* g_ImGuiContext = nullptr;
SDL_GLContext gl_context = nullptr;
static bool g_ImGuiInitialized = false;

void ImGui_initialize(SDL_Window* window) {
	if (!g_ImGuiInitialized) {
		g_ImGuiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(g_ImGuiContext);
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init();
		g_ImGuiInitialized = true;
	}
}

extern "C" void ImGui_BeginFrame(SDL_Window* window) {
	if (!g_ImGuiInitialized) {
		ImGui_initialize(window);
	} else {
		ImGui::SetCurrentContext(g_ImGuiContext);
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
}

extern "C" void ImGui_EndFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C" void ImGui_ShowToolbar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open ROM", "Ctrl+O")) {
				// Handle Open ROM action
			}
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				// Handle Exit action
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Toggle Fullscreen", "F11")) {
				// Handle Toggle Fullscreen action
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

extern "C" void ImGui_ProcessEvent(const SDL_Event* event) {
	if (!g_ImGuiInitialized)
		return;
	ImGui_ImplSDL2_ProcessEvent(event);
}
