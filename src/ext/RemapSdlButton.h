/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include "../config.h"

#ifdef __cplusplus
#include <unordered_map>
#include <map>
#include <string>

extern "C" {
#endif

enum ControllerType {
	CT_GameController,
	CT_Joystick
};

struct ControllerKey {
	int controllerID;
	enum ControllerType type;

	#ifdef __cplusplus
	bool operator==(const ControllerKey &other) const {
		return controllerID == other.controllerID && type == other.type;
	}
	#endif
};

#ifdef __cplusplus
}
namespace std {
template <>
struct hash<ControllerKey> {
	std::size_t operator()(const ControllerKey &k) const {
		return std::hash<int>()(k.controllerID) ^ (std::hash<int>()(k.type) << 1);
	}
};
}

extern std::unordered_map<ControllerKey, std::unordered_map<int, int>> controllerButtonMappings;

static const std::unordered_map<int, int> defaultButtonMapping = {
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

static const std::map<int, std::string> sdlButtonNames = {
	{SDL_CONTROLLER_BUTTON_A, "A"},
	{SDL_CONTROLLER_BUTTON_B, "B"},
	{SDL_CONTROLLER_BUTTON_X, "X"},
	{SDL_CONTROLLER_BUTTON_Y, "Y"},
	{SDL_CONTROLLER_BUTTON_BACK, "Back"},
	{SDL_CONTROLLER_BUTTON_GUIDE, "Guide"},
	{SDL_CONTROLLER_BUTTON_START, "Start"},
	{SDL_CONTROLLER_BUTTON_LEFTSTICK, "Left Stick"},
	{SDL_CONTROLLER_BUTTON_RIGHTSTICK, "Right Stick"},
	{SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "Left Shoulder"},
	{SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "Right Shoulder"},
	{SDL_CONTROLLER_BUTTON_DPAD_UP, "DPad Up"},
	{SDL_CONTROLLER_BUTTON_DPAD_DOWN, "DPad Down"},
	{SDL_CONTROLLER_BUTTON_DPAD_LEFT, "DPad Left"},
	{SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "DPad Right"}
};

static const std::map<int, std::string> internalButtonNames = {
	{kGamepadBtn_A, "A"},
	{kGamepadBtn_B, "B"},
	{kGamepadBtn_X, "X"},
	{kGamepadBtn_Y, "Y"},
	{kGamepadBtn_Back, "Back"},
	{kGamepadBtn_Guide, "Guide"},
	{kGamepadBtn_Start, "Start"},
	{kGamepadBtn_L3, "Left Stick"},
	{kGamepadBtn_R3, "Right Stick"},
	{kGamepadBtn_L1, "Left Shoulder"},
	{kGamepadBtn_R1, "Right Shoulder"},
	{kGamepadBtn_DpadUp, "DPad Up"},
	{kGamepadBtn_DpadDown, "DPad Down"},
	{kGamepadBtn_DpadLeft, "DPad Left"},
	{kGamepadBtn_DpadRight, "DPad Right"},
	{kGamepadBtn_L2, "L2 (Analog)"},
	{kGamepadBtn_R2, "R2 (Analog)"},
	{kGamepadBtn_Invalid, "Unmapped"}
};

extern "C" {
#endif

int ResolveSdlButton(struct ControllerKey controllerID, int button);
void ChangeSdlButtonMapping(struct ControllerKey controllerID, int sdlButton, int internalButton);

void saveButtonConfig();
void loadButtonConfig();

#ifdef __cplusplus
}
#endif
