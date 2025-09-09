/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include "GameRAM.h"
#include <cstring>

GameRAM game_ram;

uint8_t& GameRAM::operator[](size_t index) {
	// Custom logic to determine which player state to access will be put here later
	// For now, just mirror the original behavior
	return data[index];
}

void GameRAM::setPlayer(int player) {
	copyRAMToPlayerState(); // Store player A's state before switching to player B
	use_player2 = (player == 2);
	copyPlayerStateToRAM(); // Load player B's state into RAM
}

void GameRAM::togglePlayer() {
	copyRAMToPlayerState(); // Store current player's state before switching
	use_player2 = !use_player2;
	copyPlayerStateToRAM(); // Load the new player's state into RAM
}

void GameRAM::resetPlayerStates() {
	copyRAMToPlayerState(); // Store RAM values to current player state
	use_player2 = !use_player2; // Switch to the other player
	copyRAMToPlayerState(); // Store the same RAM values to the other player state
	use_player2 = !use_player2; // Switch back to the original player
}

void GameRAM::copyRAMToPlayerState() {
	// Player state is stored in RAM between 0x20 and 0x71 (inclusive)
	uint8_t* player_state = use_player2 ? player2_state : player1_state;
	std::memcpy(player_state, &data[0x20], 0x52); // 0x71 - 0x20 + 1 = 0x52
}

void GameRAM::copyPlayerStateToRAM() {
	// Player state is stored in RAM between 0x20 and 0x71 (inclusive)
	uint8_t* player_state = use_player2 ? player2_state : player1_state;
	std::memcpy(&data[0x20], player_state, 0x52); // 0x71 - 0x20 + 1 = 0x52
}

uint8_t* g_ram_access(size_t idx) {
	return &game_ram[idx];
}

void switch_player() {
	game_ram.togglePlayer();
}

void _test_init_multi() {
	game_ram.resetPlayerStates(); // To avoid uninitialized memory issues
}
