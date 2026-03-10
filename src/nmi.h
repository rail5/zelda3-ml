#pragma once
#include "types.h"

typedef struct JoypadInputState {
	uint8 joypad1h_last;
	uint8 joypad1l_last;
	uint8 filtered_joypad_h;
	uint8 filtered_joypad_l;
	uint8 joypad1h_last2;
	uint8 joypad1l_last2;
} JoypadInputState;

extern JoypadInputState g_joypad_state_by_player[2];

void NMI_UploadSubscreenOverlayFormer();
void NMI_UploadSubscreenOverlayLatter();
void Interrupt_NMI(uint16 joypad_input_1, uint16 joypad_input_2);
void NMI_ReadJoypads(uint16 joypad_input_1, uint16 joypad_input_2);
void NMI_ApplyJoypadStateForPlayer(int player);
void NMI_DoUpdates();
void NMI_UploadTilemap();
void NMI_UploadTilemap_doNothing();
void NMI_UploadBG3Text();
void NMI_UpdateOWScroll();
void NMI_UpdateSubscreenOverlay();
void NMI_HandleArbitraryTileMap(const uint8 *src, int i, int i_end);
void NMI_UpdateBG1Wall();
void NMI_TileMapNothing();
void NMI_UpdateLoadLightWorldMap();
void NMI_UpdateBG2Left();
void NMI_UpdateBGChar3and4();
void NMI_UpdateBGChar5and6();
void NMI_UpdateBGCharHalf();
void NMI_UpdateBGChar0();
void NMI_UpdateBGChar1();
void NMI_UpdateBGChar2();
void NMI_UpdateBGChar3();
void NMI_UpdateObjChar0();
void NMI_UpdateObjChar2();
void NMI_UpdateObjChar3();
void NMI_RunTileMapUpdateDMA(int dst);
void NMI_UploadDarkWorldMap();
void NMI_UploadGameOverText();
void NMI_UpdatePegTiles();
void NMI_UpdateStarTiles();
void HandleStripes14(const uint8 *p);
void NMI_UpdateIRQGFX();
