/**
 * Copyright (C) 2025 Andrew S. Rightenburg
 * MIT License
 */

#include "GameRAM.h"
#include <cstdint>
#include <cstring>

GameRAM game_ram;

namespace {

void CopyPlayerStateRange(uint8_t* dst, const uint8_t* src, size_t size) {
	std::memcpy(dst, src, size);
}

uint16_t ReadStateU16(const uint8_t* src) {
	uint16_t value;
	std::memcpy(&value, src, sizeof(value));
	return value;
}

void WriteStateU16(uint8_t* dst, uint16_t value) {
	std::memcpy(dst, &value, sizeof(value));
}

void AdjustStatePosition(uint8_t* core_state, uint8_t* action_state, uint8_t* prev_coord_state,
		uint8_t* special_exit_state, uint8_t* exit_state, uint8_t* cached_state,
		int16_t dx, int16_t dy) {
	WriteStateU16(core_state + 0x0, ReadStateU16(core_state + 0x0) + dy);
	WriteStateU16(core_state + 0x2, ReadStateU16(core_state + 0x2) + dx);
	WriteStateU16(action_state + 0x4, ReadStateU16(action_state + 0x4) + dx);
	WriteStateU16(action_state + 0x6, ReadStateU16(action_state + 0x6) + dy);
	WriteStateU16(prev_coord_state + 0x1, ReadStateU16(prev_coord_state + 0x1) + dx);
	WriteStateU16(prev_coord_state + 0x3, ReadStateU16(prev_coord_state + 0x3) + dy);
	WriteStateU16(special_exit_state + 0x0, ReadStateU16(special_exit_state + 0x0) + dy);
	WriteStateU16(special_exit_state + 0x2, ReadStateU16(special_exit_state + 0x2) + dx);
	WriteStateU16(exit_state + 0x0, ReadStateU16(exit_state + 0x0) + dy);
	WriteStateU16(exit_state + 0x2, ReadStateU16(exit_state + 0x2) + dx);
	WriteStateU16(cached_state + 0x4, ReadStateU16(cached_state + 0x4) + dy);
	WriteStateU16(cached_state + 0x6, ReadStateU16(cached_state + 0x6) + dx);
}

}

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
	multiplayer_initialized = true;
}

void GameRAM::ensureMultiplayerInitialized() {
	if (multiplayer_initialized)
		return;

	copyRAMToPlayerState();

	uint8_t* src_core_state = use_player2 ? player2_core_state : player1_core_state;
	uint8_t* src_action_state = use_player2 ? player2_action_state : player1_action_state;
	uint8_t* src_prev_coord_state = use_player2 ? player2_prev_coord_state : player1_prev_coord_state;
	uint8_t* src_special_exit_state = use_player2 ? player2_special_exit_state : player1_special_exit_state;
	uint8_t* src_exit_state = use_player2 ? player2_exit_state : player1_exit_state;
	uint8_t* src_cached_state = use_player2 ? player2_cached_state : player1_cached_state;

	uint8_t* dst_core_state = use_player2 ? player1_core_state : player2_core_state;
	uint8_t* dst_action_state = use_player2 ? player1_action_state : player2_action_state;
	uint8_t* dst_prev_coord_state = use_player2 ? player1_prev_coord_state : player2_prev_coord_state;
	uint8_t* dst_special_exit_state = use_player2 ? player1_special_exit_state : player2_special_exit_state;
	uint8_t* dst_exit_state = use_player2 ? player1_exit_state : player2_exit_state;
	uint8_t* dst_cached_state = use_player2 ? player1_cached_state : player2_cached_state;

	CopyPlayerStateRange(dst_core_state, src_core_state, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	CopyPlayerStateRange(dst_action_state, src_action_state, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	CopyPlayerStateRange(dst_prev_coord_state, src_prev_coord_state, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	CopyPlayerStateRange(dst_special_exit_state, src_special_exit_state, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	CopyPlayerStateRange(dst_exit_state, src_exit_state, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	CopyPlayerStateRange(dst_cached_state, src_cached_state, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
	AdjustStatePosition(dst_core_state, dst_action_state, dst_prev_coord_state, dst_special_exit_state,
		dst_exit_state, dst_cached_state, 16, 0);

	multiplayer_initialized = true;
}

void GameRAM::invalidateMultiplayerState() {
	multiplayer_initialized = false;
}

void GameRAM::copyRAMToPlayerState() {
	uint8_t* core_state = use_player2 ? player2_core_state : player1_core_state;
	uint8_t* action_state = use_player2 ? player2_action_state : player1_action_state;
	uint8_t* prev_coord_state = use_player2 ? player2_prev_coord_state : player1_prev_coord_state;
	uint8_t* special_exit_state = use_player2 ? player2_special_exit_state : player1_special_exit_state;
	uint8_t* exit_state = use_player2 ? player2_exit_state : player1_exit_state;
	uint8_t* cached_state = use_player2 ? player2_cached_state : player1_cached_state;

	CopyPlayerStateRange(core_state, &data[0x20], GAME_RAM_PLAYER_CORE_STATE_SIZE);
	CopyPlayerStateRange(action_state, &data[0x2D8], GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	CopyPlayerStateRange(prev_coord_state, &data[0xFC1], GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	CopyPlayerStateRange(special_exit_state, &data[0xC108], GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	CopyPlayerStateRange(exit_state, &data[0xC148], GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	CopyPlayerStateRange(cached_state, &data[0xC180], GAME_RAM_PLAYER_CACHED_STATE_SIZE);
}

void GameRAM::copyPlayerStateToRAM() {
	uint8_t* core_state = use_player2 ? player2_core_state : player1_core_state;
	uint8_t* action_state = use_player2 ? player2_action_state : player1_action_state;
	uint8_t* prev_coord_state = use_player2 ? player2_prev_coord_state : player1_prev_coord_state;
	uint8_t* special_exit_state = use_player2 ? player2_special_exit_state : player1_special_exit_state;
	uint8_t* exit_state = use_player2 ? player2_exit_state : player1_exit_state;
	uint8_t* cached_state = use_player2 ? player2_cached_state : player1_cached_state;

	CopyPlayerStateRange(&data[0x20], core_state, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	CopyPlayerStateRange(&data[0x2D8], action_state, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	CopyPlayerStateRange(&data[0xFC1], prev_coord_state, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	CopyPlayerStateRange(&data[0xC108], special_exit_state, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	CopyPlayerStateRange(&data[0xC148], exit_state, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	CopyPlayerStateRange(&data[0xC180], cached_state, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
}

/**
 * SAVESTATES
 *
 * Savestates are mostly handled by the main program code.
 * But since we're storing extra state here, we should report what we have
 * These functions will be called by the main code (src/zelda_rtl.c: StateRecorder_Save / StateRecorder_Load)
 */
uint8_t* GameRAM::getPlayerStates() {
	uint8_t* snapshot = new uint8_t[GAME_RAM_SNAPSHOT_SIZE];
	// Should call g_ram_snapshot_free() when done

	copyRAMToPlayerState(); // Ensure current player's state is up to date

	size_t offset = 0;
	CopyPlayerStateRange(snapshot + offset, player1_core_state, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CORE_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player1_action_state, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	offset += GAME_RAM_PLAYER_ACTION_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player1_prev_coord_state, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	offset += GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player1_special_exit_state, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player1_exit_state, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_EXIT_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player1_cached_state, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CACHED_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_core_state, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CORE_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_action_state, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	offset += GAME_RAM_PLAYER_ACTION_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_prev_coord_state, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	offset += GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_special_exit_state, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_exit_state, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_EXIT_STATE_SIZE;
	CopyPlayerStateRange(snapshot + offset, player2_cached_state, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CACHED_STATE_SIZE;
	snapshot[offset] = use_player2 ? 1 : 0;
	return snapshot;
}

void GameRAM::loadState(const uint8_t* state) {
	size_t offset = 0;
	CopyPlayerStateRange(player1_core_state, state + offset, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CORE_STATE_SIZE;
	CopyPlayerStateRange(player1_action_state, state + offset, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	offset += GAME_RAM_PLAYER_ACTION_STATE_SIZE;
	CopyPlayerStateRange(player1_prev_coord_state, state + offset, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	offset += GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE;
	CopyPlayerStateRange(player1_special_exit_state, state + offset, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE;
	CopyPlayerStateRange(player1_exit_state, state + offset, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_EXIT_STATE_SIZE;
	CopyPlayerStateRange(player1_cached_state, state + offset, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CACHED_STATE_SIZE;
	CopyPlayerStateRange(player2_core_state, state + offset, GAME_RAM_PLAYER_CORE_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CORE_STATE_SIZE;
	CopyPlayerStateRange(player2_action_state, state + offset, GAME_RAM_PLAYER_ACTION_STATE_SIZE);
	offset += GAME_RAM_PLAYER_ACTION_STATE_SIZE;
	CopyPlayerStateRange(player2_prev_coord_state, state + offset, GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE);
	offset += GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE;
	CopyPlayerStateRange(player2_special_exit_state, state + offset, GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE;
	CopyPlayerStateRange(player2_exit_state, state + offset, GAME_RAM_PLAYER_EXIT_STATE_SIZE);
	offset += GAME_RAM_PLAYER_EXIT_STATE_SIZE;
	CopyPlayerStateRange(player2_cached_state, state + offset, GAME_RAM_PLAYER_CACHED_STATE_SIZE);
	offset += GAME_RAM_PLAYER_CACHED_STATE_SIZE;
	use_player2 = (state[offset] != 0);
	multiplayer_initialized = true;
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

void ensure_multi_initialized() {
	game_ram.ensureMultiplayerInitialized();
}

void invalidate_multi_init() {
	game_ram.invalidateMultiplayerState();
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
