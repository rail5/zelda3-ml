/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include "GameRAM.h"

GameRAM game_ram;

uint8_t& GameRAM::operator[](size_t index) {
	// Custom logic to determine which player state to access will be put here later
	// For now, just mirror the original behavior
	return data[index];
}

uint8_t* g_ram_access(size_t idx) {
	return &game_ram[idx];
}
