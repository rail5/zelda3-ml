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
extern std::unordered_map<int, int> buttonMapping;

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

int RemapSdlButton(int button);
void ChangeSdlButtonMapping(int sdlButton, int internalButton);

void saveButtonConfig();
void loadButtonConfig();

#ifdef __cplusplus
}
#endif
