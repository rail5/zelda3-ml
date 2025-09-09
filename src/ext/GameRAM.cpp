/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include "GameRAM.h"
#include <cstdint>
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

/**
 * SAVESTATES
 *
 * Savestates are mostly handled by the main program code.
 * But since we're storing extra state here, we should report what we have
 * These functions will be called by the main code (src/zelda_rtl.c: StateRecorder_Save / StateRecorder_Load)
 */
uint8_t* GameRAM::getPlayerStates() {
	// Total size: 0x52 (player 1) + 0x52 (player 2) + 1 (which player) = 0xA5
	uint8_t* snapshot = new uint8_t[0xA5]; // Caller is responsible for deleting this memory
	// Should call g_ram_snapshot_free() when done

	copyRAMToPlayerState(); // Ensure current player's state is up to date

	std::memcpy(snapshot, player1_state, 0x52);
	std::memcpy(snapshot + 0x52, player2_state, 0x52);
	snapshot[0xA4] = use_player2 ? 1 : 0;
	return snapshot;
}

void GameRAM::loadState(const uint8_t* state) {
	// Expecting a buffer of size 0xA5
	std::memcpy(player1_state, state, 0x52);
	std::memcpy(player2_state, state + 0x52, 0x52);
	use_player2 = (state[0xA4] != 0);
	copyPlayerStateToRAM(); // Load the selected player's state into RAM
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

uint8_t* g_ram_snapshot_for_savestate() {
	return game_ram.getPlayerStates();
}

void load_g_ram_snapshot_from_savestate(const uint8_t* snapshot) {
	game_ram.loadState(snapshot);
}

void g_ram_snapshot_free(uint8_t* ptr) {
	delete[] ptr;
}
