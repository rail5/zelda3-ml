/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * GNU General Public License v3.0 or later
 */

#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <SDL2/SDL_gamecontroller.h>
#include "RemapSdlButton.h"
#include "../config.h"

/**
 * Declare a global unordered_map to hold the button mappings.
 * This map can be modified at runtime if needed
 */

inline std::unordered_map<int, int> buttonMapping = {
	{SDL_CONTROLLER_BUTTON_A, kGamepadBtn_A},
	{SDL_CONTROLLER_BUTTON_B, kGamepadBtn_B},
	{SDL_CONTROLLER_BUTTON_X, kGamepadBtn_X},
	{SDL_CONTROLLER_BUTTON_Y, kGamepadBtn_Y},
	{SDL_CONTROLLER_BUTTON_BACK, kGamepadBtn_Back},
	{SDL_CONTROLLER_BUTTON_GUIDE, kGamepadBtn_Guide},
	{SDL_CONTROLLER_BUTTON_START, kGamepadBtn_Start},
	{SDL_CONTROLLER_BUTTON_LEFTSTICK, kGamepadBtn_L3},
	{SDL_CONTROLLER_BUTTON_RIGHTSTICK, kGamepadBtn_R3},
	{SDL_CONTROLLER_BUTTON_LEFTSHOULDER, kGamepadBtn_L1},
	{SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, kGamepadBtn_R1},
	{SDL_CONTROLLER_BUTTON_DPAD_UP, kGamepadBtn_DpadUp},
	{SDL_CONTROLLER_BUTTON_DPAD_DOWN, kGamepadBtn_DpadDown},
	{SDL_CONTROLLER_BUTTON_DPAD_LEFT, kGamepadBtn_DpadLeft},
	{SDL_CONTROLLER_BUTTON_DPAD_RIGHT, kGamepadBtn_DpadRight}
};

extern "C" int RemapSdlButton(int button) {
	auto it = buttonMapping.find(button);
	if (it != buttonMapping.end()) {
		return it->second;
	}
	return kGamepadBtn_Invalid;
}

extern "C" void ChangeSdlButtonMapping(int sdlButton, int internalButton) {
	buttonMapping[sdlButton] = internalButton;
}

/**
 * We might like to store these button mappings in a config file so changes to the default mapping can be saved
 */

std::filesystem::path GetConfigFilePath() {
	const char* homeDir = std::getenv("HOME");
	if (!homeDir) {
		homeDir = std::getenv("USERPROFILE"); // Windows fallback
	}
	if (!homeDir) {
		throw std::runtime_error("Unable to determine home directory for config file.");
	}
	return std::filesystem::path(homeDir) / ".zelda3_button_config";
}

extern "C" void saveButtonConfig() {
	std::filesystem::path configPath = GetConfigFilePath();
	std::ofstream configFile(configPath);
	if (!configFile.is_open()) {
		throw std::runtime_error("Unable to open config file for writing.");
	}
	for (const auto& pair : buttonMapping) {
		configFile << pair.first << "\t" << pair.second << "\n";
	}
}

extern "C" void loadButtonConfig() {
	std::filesystem::path configPath = GetConfigFilePath();
	if (!std::filesystem::exists(configPath)) {
		return; // No config file to load
	}
	std::ifstream configFile(configPath);
	if (!configFile.is_open()) {
		throw std::runtime_error("Unable to open config file for reading.");
	}
	int sdlButton, internalButton;
	while (configFile >> sdlButton >> internalButton) {
		ChangeSdlButtonMapping(sdlButton, internalButton);
	}
}
