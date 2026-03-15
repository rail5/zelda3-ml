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

#define GAME_RAM_PLAYER_CORE_STATE_SIZE 0x52
#define GAME_RAM_PLAYER_MID_STATE_SIZE 0x18
#define GAME_RAM_PLAYER_ACTION_STATE_SIZE 0xA8
#define GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE 0x5
#define GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE 0x4
#define GAME_RAM_PLAYER_EXIT_STATE_SIZE 0x4
#define GAME_RAM_PLAYER_CACHED_STATE_SIZE 0x27
#define GAME_RAM_SNAPSHOT_SIZE ((GAME_RAM_PLAYER_CORE_STATE_SIZE + GAME_RAM_PLAYER_MID_STATE_SIZE + GAME_RAM_PLAYER_ACTION_STATE_SIZE + \
	GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE + GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE + \
	GAME_RAM_PLAYER_EXIT_STATE_SIZE + GAME_RAM_PLAYER_CACHED_STATE_SIZE) * 2 + 1)

#ifdef __cplusplus
#include <cstddef>
#include <filesystem>
extern "C" {
#endif

struct GameRAM {
	uint8_t data[131072];

	uint8_t player1_core_state[GAME_RAM_PLAYER_CORE_STATE_SIZE];
	uint8_t player1_mid_state[GAME_RAM_PLAYER_MID_STATE_SIZE];
	uint8_t player1_action_state[GAME_RAM_PLAYER_ACTION_STATE_SIZE];
	uint8_t player1_prev_coord_state[GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE];
	uint8_t player1_special_exit_state[GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE];
	uint8_t player1_exit_state[GAME_RAM_PLAYER_EXIT_STATE_SIZE];
	uint8_t player1_cached_state[GAME_RAM_PLAYER_CACHED_STATE_SIZE];

	uint8_t player2_core_state[GAME_RAM_PLAYER_CORE_STATE_SIZE];
	uint8_t player2_mid_state[GAME_RAM_PLAYER_MID_STATE_SIZE];
	uint8_t player2_action_state[GAME_RAM_PLAYER_ACTION_STATE_SIZE];
	uint8_t player2_prev_coord_state[GAME_RAM_PLAYER_PREV_COORD_STATE_SIZE];
	uint8_t player2_special_exit_state[GAME_RAM_PLAYER_SPECIAL_EXIT_STATE_SIZE];
	uint8_t player2_exit_state[GAME_RAM_PLAYER_EXIT_STATE_SIZE];
	uint8_t player2_cached_state[GAME_RAM_PLAYER_CACHED_STATE_SIZE];

	bool use_player2;
	bool multiplayer_initialized;

	#ifdef __cplusplus
	uint8_t& operator[](size_t index);

	GameRAM() : data{}, player1_core_state{}, player1_mid_state{}, player1_action_state{}, player1_prev_coord_state{}, player1_special_exit_state{}, player1_exit_state{}, player1_cached_state{}, player2_core_state{}, player2_mid_state{}, player2_action_state{}, player2_prev_coord_state{}, player2_special_exit_state{}, player2_exit_state{}, player2_cached_state{}, use_player2(false), multiplayer_initialized(false) {} // zero-initialize

	void setPlayer(int player); // 1 or 2
	void togglePlayer();

	void resetPlayerStates();
	void ensureMultiplayerInitialized();
	void invalidateMultiplayerState();

	uint8_t* getPlayerStates(); // Size: GAME_RAM_SNAPSHOT_SIZE
	void loadState(const uint8_t* state);

	private:
	void copyPlayerStateToRAM();
	void copyRAMToPlayerState();
	#endif
};

extern struct GameRAM game_ram;
uint8_t* g_ram_access(size_t idx);

void switch_player();
void _test_init_multi();
void ensure_multi_initialized();
void invalidate_multi_init();

uint8_t* g_ram_snapshot_for_savestate();
void load_g_ram_snapshot_from_savestate(const uint8_t* snapshot);
void g_ram_snapshot_free(uint8_t* ptr);

#ifdef __cplusplus
}
#endif
