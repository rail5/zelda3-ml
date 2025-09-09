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
#include <filesystem>
extern "C" {
#endif

struct GameRAM {
	uint8_t data[131072];

	uint8_t player1_state[0x52];
	uint8_t player2_state[0x52];

	bool use_player2;

	#ifdef __cplusplus
	uint8_t& operator[](size_t index);

	GameRAM() : data{}, player1_state{}, player2_state{}, use_player2(false) {} // zero-initialize

	void setPlayer(int player); // 1 or 2
	void togglePlayer();

	void resetPlayerStates();

	uint8_t* getPlayerStates(); // Size: 0xA5
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

uint8_t* g_ram_snapshot_for_savestate();
void load_g_ram_snapshot_from_savestate(const uint8_t* snapshot);
void g_ram_snapshot_free(uint8_t* ptr);

#ifdef __cplusplus
}
#endif
