/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include <SDL2/SDL_video.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGui_bridge.h"

#include "RemapSdlButton.h"

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

static bool show_about_dialog = false;
static bool show_controller_mapping_dialog = false;

bool remapping_active = false;
int remapping_internal_button = kGamepadBtn_Invalid;

extern "C" void ImGui_ShowToolbar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				// Handle Exit action
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Controller Mapping", "Ctrl+M")) {
				// Bring up controller mapping dialog
				show_controller_mapping_dialog = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Toggle Fullscreen", "F11")) {
				// Handle Toggle Fullscreen action
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About", "F1")) {
				// Show about dialog
				show_about_dialog = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		if (show_about_dialog) {
			ImGui::OpenPopup("About");
			show_about_dialog = false;
		}

		if (show_controller_mapping_dialog) {
			ImGui::OpenPopup("Controller Mapping");
			show_controller_mapping_dialog = false;
		}

		if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("The Legend of Zelda: A Link to the Past PC Port\n");
			ImGui::Separator();
			ImGui::Text("Forked by Andrew S. Rightenburg\n");
			ImGui::Text("Original by snesrev, elzo_d\n");
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Controller Mapping", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Dummy(ImVec2(500, 0)); // Forces minimum width
			ImGui::Text("Controller Mapping Configuration\n");
			ImGui::Separator();
			
			// Table header
			ImGui::Columns(3, "ControllerMappingTable");
			ImGui::Text("Internal Button"); ImGui::NextColumn();
			ImGui::Text("Controller Mapping"); ImGui::NextColumn();
			ImGui::Text("Change"); ImGui::NextColumn();
			ImGui::Separator();

			// Iterate over internal buttons in order
			for (const auto& internalPair : internalButtonNames) {
			    int internalButton = internalPair.first;

				if (internalButton == kGamepadBtn_Invalid)
					continue;

			    const std::string& internalName = internalPair.second;

			    // Find which SDL button is mapped to this internal button
			    int mappedSdlButton = -1;
			    for (const auto& pair : buttonMapping) {
			        if (pair.second == internalButton) {
			            mappedSdlButton = pair.first;
			            break;
			        }
			    }

			    std::string sdlName = (sdlButtonNames.count(mappedSdlButton)) ? sdlButtonNames.at(mappedSdlButton) : "Unmapped";

			    ImGui::Text("%s", internalName.c_str());
			    ImGui::NextColumn();
			    ImGui::Text("%s", sdlName.c_str());
			    ImGui::NextColumn();

			    if (remapping_active && remapping_internal_button == internalButton) {
			        ImGui::Text("Press a button...");
			    } else {
			        if (ImGui::Button(("Change##" + std::to_string(internalButton)).c_str())) {
			            remapping_active = true;
			            remapping_internal_button = internalButton;
			        }
			    }
			    ImGui::NextColumn();
			}
			ImGui::Columns(1);

			if (ImGui::Button("Save Configuration")) {
				saveButtonConfig();
			}

			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

extern "C" void ImGui_ProcessEvent(const SDL_Event* event) {
	if (!g_ImGuiInitialized)
		return;
	ImGui_ImplSDL2_ProcessEvent(event);
}

extern "C" bool ImGui_WantCaptureMouse() {
	if (!g_ImGuiInitialized)
		return false;
	return ImGui::GetIO().WantCaptureMouse;
}

extern "C" bool ImGui_WantCaptureKeyboard() {
	if (!g_ImGuiInitialized)
		return false;
	return ImGui::GetIO().WantCaptureKeyboard;
}
