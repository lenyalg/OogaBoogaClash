#ifndef HUD_SYSTEM_H
#define HUD_SYSTEM_H

#include "raylib.h"
#include <stdio.h>

typedef struct {
    float max_health;
    float *current_health;
    int x;
    int y;
    int total_width;
    int start_width;
    int end_width;
    int middle_segment_width;
    int fill_direction; // 1: Gauche->Droite, -1: Droite->Gauche
    Texture2D bg_texture;
    Texture2D start_texture;
    Texture2D middle_texture;
    Texture2D end_texture;
    Texture2D over_texture;
} HealthBar;

typedef struct {
    int x;
    int y;
    int slot_width;
    int slot_height;
    int slot_count;
    Texture2D left_texture;
    Texture2D right_texture;
    Texture2D middle_texture;
    Texture2D item_texture;
} InventoryBar;

HealthBar HUD_InitHealthBar(float *max_hp, int x, int y, int total_width, int start_width, int end_width, int middle_segment_width, int fill_direction);
void HUD_DrawHealthBar(const HealthBar* bar);
void HUD_HandleInput(HealthBar* player1_bar, HealthBar* player2_bar);
void HUD_UnloadHealthBar(HealthBar* bar);

InventoryBar HUD_InitInventoryBar(int x, int y, int slot_width, int slot_height, int slot_count);
void HUD_DrawInventoryBar(const InventoryBar* bar, int dash_cooldown_frames, int character_type, Texture2D sprite_texture);
void HUD_UnloadInventoryBar(InventoryBar* bar);

void HUD_DrawStock(int stock, int x, int y, Texture2D stock_texture, bool isP2);
char* GetPortraitPath(int character_type);

#endif