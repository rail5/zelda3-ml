#pragma once
#include "types.h"

typedef struct SwordResult {
  uint8 r6;
  uint8 r12;
} SwordResult;

typedef struct LinkDmaSelectors {
  uint16 graphics_index;
  uint16 var1;
  uint16 var2;
  uint8 var3;
  uint8 var4;
  uint8 var5;
} LinkDmaSelectors;

extern LinkDmaSelectors g_link_dma_selectors_by_player[2];

bool PlayerOam_WantInvokeSword();
void CalculateSwordHitBox();
void LinkOam_Main();
uint8 FindMostSignificantBit(uint8 v);
bool LinkOam_SetWeaponVRAMOffsets(int r2, SwordResult *sr);
bool LinkOam_SetEquipmentVRAMOffsets(int r2, SwordResult *sr);
int LinkOam_CalculateSwordSparklePosition(int oam_pos, uint8 oam_x, uint8 oam_y);
void LinkOam_UnusedWeaponSettings(int r4loc, uint8 oam_x, uint8 oam_y);
void LinkOam_DrawDungeonFallShadow(int r4loc, uint8 xcoord);
void LinkOam_DrawFootObject(int r4loc, uint8 oam_x, uint8 oam_y);
void LinkOam_CalculateXOffsetRelativeLink(uint8 x);
