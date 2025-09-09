/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "RemapSdlButton.h"

/**
 * Declare a global unordered_map to hold the button mappings.
 * This map can be modified at runtime if needed
 */

std::unordered_map<ControllerKey, std::unordered_map<int, int>> controllerButtonMappings; // Key: controller ID, Value: button map

extern "C" int ResolveSdlButton(ControllerKey controllerID, int button) {
	auto ctrlIt = controllerButtonMappings.find(controllerID);
	if (ctrlIt != controllerButtonMappings.end()) {
		auto btnIt = ctrlIt->second.find(button);
		if (btnIt != ctrlIt->second.end()) {
			return btnIt->second;
		}
	} else {
		// Pull from default mapping if no custom mapping exists for this controller
		auto defIt = defaultButtonMapping.find(button);
		if (defIt != defaultButtonMapping.end()) {
			return defIt->second;
		}
	}

	return kGamepadBtn_Invalid;
}

extern "C" void ChangeSdlButtonMapping(ControllerKey controllerID, int sdlButton, int internalButton) {
	controllerButtonMappings[controllerID][sdlButton] = internalButton;
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
	for (const auto& ctrlPair : controllerButtonMappings) {
		ControllerKey controller = ctrlPair.first;
		int controllerID = controller.controllerID;
		ControllerType controllerType = controller.type;
		for (const auto& btnPair : ctrlPair.second) {
			int sdlButton = btnPair.first;
			int internalButton = btnPair.second;
			configFile << controllerType << "\t" << controllerID << "\t" << sdlButton << "\t" << internalButton << "\n";
		}
	}
}

extern "C" void loadButtonConfig() {
	std::filesystem::path configPath = GetConfigFilePath();
	if (!std::filesystem::exists(configPath)) {
		return; // No config file to load
	}
	std::ifstream configFile(configPath);
	if (!configFile.is_open()) {
		return; // Fail open, just use defaults
	}
	int controllerTypeInt;
	int controllerID, sdlButton, internalButton;
	while (configFile >> controllerTypeInt >> controllerID >> sdlButton >> internalButton) {
		ControllerKey key;
		key.type = static_cast<ControllerType>(controllerTypeInt);
		key.controllerID = controllerID;
		ChangeSdlButtonMapping(key, sdlButton, internalButton);
	}
}
