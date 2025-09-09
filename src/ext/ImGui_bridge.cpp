/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include <SDL2/SDL_video.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <filesystem>

#include "ImGui_bridge.h"
#include "RemapSdlButton.h"

#include "../zelda_rtl.h"
#include "../fullscreen.h"

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

std::filesystem::path getSaveStatePath(int slot) {
	const char* homeDir = std::getenv("HOME");
	if (!homeDir) {
		homeDir = std::getenv("USERPROFILE"); // Windows fallback
	}
	if (!homeDir) {
		throw std::runtime_error("Unable to determine home directory for config file.");
	}

	std::string saveStateFileName = ".zelda3_" + std::to_string(slot) + ".sav";
	return std::filesystem::path(homeDir) / saveStateFileName;
}

static bool show_about_dialog = false;
static bool show_controller_mapping_dialog = false;

bool remapping_active = false;
int remapping_internal_button = kGamepadBtn_Invalid;

extern "C" void ImGui_ShowToolbar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				exit(0);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Controller Mapping", "")) {
				// Bring up controller mapping dialog
				show_controller_mapping_dialog = true;
			}

			if (ImGui::BeginMenu("Save State")) {
				if (ImGui::MenuItem("Save to Slot 1", "Shift+F1")) {
					SaveLoadSlot(kSaveLoad_Save, 1);
				}
				if (ImGui::MenuItem("Save to Slot 2", "Shift+F2")) {
					SaveLoadSlot(kSaveLoad_Save, 2);
				}
				if (ImGui::MenuItem("Save to Slot 3", "Shift+F3")) {
					SaveLoadSlot(kSaveLoad_Save, 3);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Load State")) {
				if (ImGui::MenuItem("Load from Slot 1", "F1")) {
					SaveLoadSlot(kSaveLoad_Load, 1);
				}
				if (ImGui::MenuItem("Load from Slot 2", "F2")) {
					SaveLoadSlot(kSaveLoad_Load, 2);
				}
				if (ImGui::MenuItem("Load from Slot 3", "F3")) {
					SaveLoadSlot(kSaveLoad_Load, 3);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Toggle Fullscreen", "Alt+Enter")) {
				Fullscreen_Toggle();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About", "")) {
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

			int numJoysticks = SDL_NumJoysticks();

			if (ImGui::BeginTabBar("ControllersTabBar")) {
				for (int i = 0; i < numJoysticks; i++) {
					SDL_Joystick* joystick = SDL_JoystickOpen(i);
					if (joystick) {
						std::string controllerName = SDL_JoystickName(joystick);
						int controllerID = SDL_JoystickInstanceID(joystick);
						ControllerType controllerType = SDL_IsGameController(i) ? CT_GameController : CT_Joystick;
						ControllerKey whichController;
						whichController.controllerID = controllerID;
						whichController.type = controllerType;
						if (controllerName.empty()) {
							controllerName = "Controller " + std::to_string(i);
						}
						if (ImGui::BeginTabItem(controllerName.c_str())) {
							ImGui::Text("Mapping for: %s", controllerName.c_str());
							ImGui::Separator();

							auto& buttonMapping = controllerButtonMappings[whichController];
							if (buttonMapping.empty()) {
								buttonMapping = defaultButtonMapping; // Initialize with default if empty
							}

							// Display mapping table
							std::string columnLabel = "ControllerMappingTable" + std::to_string(controllerID);
							ImGui::Columns(3, columnLabel.c_str());
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
							ImGui::EndTabItem();
						}
					}
				}
				ImGui::EndTabBar();
			}

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
