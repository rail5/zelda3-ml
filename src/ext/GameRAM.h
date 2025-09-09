/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

/**
 * The original game RAM setup was to have a single C-style array of uint8_t totalling 128KB
 * This was evidently done to mimic the original hardware.
 * 
 * This interface provides a C-accessible API to read and write to the game RAM,
 * With one extra feature: multiple copies of the player state variable
 * When accessing regions associated with the player state, the structure will check the values of certain flags
 * to determine *which* player state to read or write.
 *
 * Ideally, this will allow for easier implementation of multiplayer.
 */

#pragma once
#include <stdint.h>

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#endif

struct GameRAM {
	uint8_t data[131072];
	#ifdef __cplusplus
	uint8_t& operator[](size_t index);
	#endif
};

extern struct GameRAM game_ram;
uint8_t* g_ram_access(size_t idx);

#ifdef __cplusplus
}
#endif
