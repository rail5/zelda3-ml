#include <unordered_map>
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
