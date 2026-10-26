#include "hud_system.h"
#include "raylib.h"
#include <stdio.h>
#include "debug.h"
#include "sprite_manager.h"
#include "common.h"

// Initialise la barre et charge les textures
HealthBar HUD_InitHealthBar(float* max_hp, int x, int y, int total_width, int start_width, int end_width, int middle_segment_width, int fill_direction) {
    HealthBar bar;
    bar.max_health = *max_hp;
    bar.current_health = max_hp;
    bar.x = x;
    bar.y = y;
    bar.total_width = total_width;
    bar.start_width = start_width;
    bar.end_width = end_width;
    bar.middle_segment_width = middle_segment_width;
    bar.fill_direction = fill_direction;
    bar.bg_texture = LoadTexture("assets/hud/health-bar-backgound.png");
    bar.start_texture = LoadTexture("assets/hud/bar-start.png");
    bar.middle_texture = LoadTexture("assets/hud/bar-middle.png");
    bar.end_texture = LoadTexture("assets/hud/bar-end.png");
    bar.over_texture = LoadTexture("assets/hud/health-bar-top-1.png");
    
    // Auto-ajustement des largeurs
    if (bar.start_texture.id != 0) bar.start_width = bar.start_texture.width;
    if (bar.middle_texture.id != 0) bar.middle_segment_width = bar.middle_texture.width;
    if (bar.end_texture.id != 0) bar.end_width = bar.end_texture.width;
    
    return bar;
}

// Dessine la barre segmentée
void HUD_DrawHealthBar(const HealthBar* bar) {
    if (!bar) return;
    
    float health_percentage = *(bar->current_health) / bar->max_health;
    if (health_percentage < 0.0f) health_percentage = 0.0f;
    if (health_percentage > 1.0f) health_percentage = 1.0f;
    
    int fill_width = (int)(bar->total_width * health_percentage);
    
    // Dessiner le fond
    if (bar->bg_texture.id != 0) {
        DrawTexturePro(bar->bg_texture, 
            (Rectangle){0, 0, (float)bar->bg_texture.width, (float)bar->bg_texture.height},
            (Rectangle){(float)bar->x, (float)bar->y, (float)bar->total_width, (float)bar->bg_texture.height},
            (Vector2){0,0}, 0.0f, WHITE);
    }
    
    // Dessiner les segments
    if (fill_width > 0) {
        if (bar->fill_direction == 1) {
            // Joueur 1: gauche vers la droite
            if (bar->start_texture.id != 0) DrawTexture(bar->start_texture, bar->x, bar->y, WHITE);
            
            int middle_width = fill_width - bar->start_width - bar->end_width;
            if (middle_width < 0) middle_width = 0;
            
            int segments_count = middle_width / bar->middle_segment_width;
            int remainder = middle_width % bar->middle_segment_width;

            if (bar->middle_texture.id != 0) {
                for (int i = 0; i < segments_count; i++) {
                    DrawTexture(bar->middle_texture, bar->x + bar->start_width + (i * bar->middle_segment_width), bar->y, WHITE);
                }
                if (remainder > 0) {
                    DrawTextureRec(bar->middle_texture, 
                        (Rectangle){0, 0, (float)remainder, (float)bar->middle_texture.height}, 
                        (Vector2){(float)(bar->x + bar->start_width + segments_count * bar->middle_segment_width), (float)bar->y}, 
                        WHITE);
                }
            }
            
            if (bar->end_texture.id != 0) DrawTexture(bar->end_texture, bar->x + fill_width - bar->end_width, bar->y, WHITE);
        } 
        else {
            // Joueur 2: droite vers gauche
            if (bar->end_texture.id != 0) DrawTexture(bar->end_texture, bar->x + bar->total_width - bar->end_width, bar->y, WHITE);
            
            int middle_width = fill_width - bar->start_width - bar->end_width;
            if (middle_width < 0) middle_width = 0;
            
            int segments_count = middle_width / bar->middle_segment_width;
            int remainder = middle_width % bar->middle_segment_width;

            if (bar->middle_texture.id != 0) {
                for (int i = 0; i < segments_count; i++) {
                    DrawTexture(bar->middle_texture, bar->x + bar->total_width - bar->end_width - ((i + 1) * bar->middle_segment_width), bar->y, WHITE);
                }
                if (remainder > 0) {
                    DrawTextureRec(bar->middle_texture, 
                        (Rectangle){(float)(bar->middle_texture.width - remainder), 0, (float)remainder, (float)bar->middle_texture.height}, 
                        (Vector2){(float)(bar->x + bar->total_width - bar->end_width - segments_count * bar->middle_segment_width - remainder), (float)bar->y}, 
                        WHITE);
                }
            }
            
            if (bar->start_texture.id != 0) DrawTexture(bar->start_texture, bar->x + bar->total_width - fill_width, bar->y, WHITE);
        }
    }
    
    // Afficher la vie
    DrawText(TextFormat("HP: %.0f/%.0f", *(bar->current_health), bar->max_health), bar->x, bar->y - 30, 20, WHITE);
    
    // Afficher l'overlay par-dessus
    if (bar->over_texture.id != 0) {
        int over_width = (int)(bar->total_width * 1.1f);
        DrawTexturePro(bar->over_texture, 
            (Rectangle){0, 0, (float)bar->over_texture.width, (float)bar->over_texture.height}, 
            (Rectangle){(float)(bar->x - (over_width - bar->total_width) / 2), (float)bar->y, (float)over_width, (float)bar->bg_texture.height}, 
            (Vector2){0,0}, 0.0f, WHITE);
    }
}

// Décharge les textures
void HUD_UnloadHealthBar(HealthBar* bar) {
    if (!bar) return;
    UnloadTexture(bar->bg_texture);
    UnloadTexture(bar->start_texture);
    UnloadTexture(bar->middle_texture);
    UnloadTexture(bar->end_texture);
    UnloadTexture(bar->over_texture);
}

InventoryBar HUD_InitInventoryBar(int x, int y, int slot_width, int slot_height, int slot_count) {
    InventoryBar bar;
    bar.x = x;
    bar.y = y;
    bar.slot_width = slot_width;
    bar.slot_height = slot_height;
    bar.slot_count = slot_count;
    bar.left_texture = LoadTexture("assets/hud/inventory-left.png");
    bar.right_texture = LoadTexture("assets/hud/inventory-right.png");
    bar.middle_texture = LoadTexture("assets/hud/inventory-middle.png");
    bar.item_texture = LoadTexture("assets/hud/inventory-case.png");
    return bar;
}

void HUD_DrawInventoryBar(const InventoryBar* bar, int dash_cooldown_frames, int character_type, Texture2D sprite_texture) {
    if (!bar) return;
    
    int current_x = bar->x;
    
    // Draw structure
    if (bar->left_texture.id != 0) {
        DrawTexture(bar->left_texture, current_x, bar->y, WHITE);
        current_x += bar->left_texture.width;
    }
    
    for (int i = 0; i < bar->slot_count; i++) {
        if (bar->middle_texture.id != 0) {
            DrawTexture(bar->middle_texture, current_x, bar->y, WHITE);
            current_x += bar->middle_texture.width;
        }
    }
    
    if (bar->right_texture.id != 0) {
        DrawTexture(bar->right_texture, current_x, bar->y, WHITE);
    }
    
    // Item case logic
    if (bar->item_texture.id != 0) {
        int slot_width = (bar->middle_texture.id != 0) ? bar->middle_texture.width : bar->slot_width;
        int slot_height = (bar->middle_texture.id != 0) ? bar->middle_texture.height : bar->slot_height;
        
        int left_w = (bar->left_texture.id != 0) ? bar->left_texture.width : 0;
        int middle_w = (bar->middle_texture.id != 0) ? bar->middle_texture.width : bar->slot_width;
        
        bool is_right = (bar->x > (WINDOWWIDTH / 2));
        int dash_slot_x = is_right ? (bar->x + left_w + (bar->slot_count - 1) * middle_w) : (bar->x + left_w);
        
        float target_h = slot_height * 0.7f;
        float scale = target_h / (float)bar->item_texture.height;
        float item_w = bar->item_texture.width * scale;
        float item_h = bar->item_texture.height * scale;

        float item_x = (float)dash_slot_x + (slot_width - item_w) / 2.0f;
        float item_y = (float)bar->y + (slot_height - item_h) / 2.0f;

        DrawTextureEx(bar->item_texture, (Vector2){ item_x, item_y }, 0.0f, scale, WHITE);

        // Character sprite
        if (sprite_texture.id != 0) {
            SpriteMetadata *metadata = GetSpriteMetadataPtr((CharacterType)character_type);
            if (metadata) {
                int dive_col = 0, dive_row = 0;
                if (character_type <= 1) {
                    dive_col = 4; dive_row = 3;
                } else { 
                    dive_col = 1; dive_row = 3;
                }
                
                Rectangle src = { 
                    (float)(dive_col * metadata->frameWidth), 
                    (float)(dive_row * metadata->frameHeight), 
                    metadata->frameWidth, 
                    metadata->frameHeight 
                };
                
                float sprite_scale = (slot_height * 0.5f) / metadata->frameHeight;
                float sprite_w = metadata->frameWidth * sprite_scale;
                float sprite_h = metadata->frameHeight * sprite_scale;
                
                Rectangle dst = {
                    (float)dash_slot_x + (slot_width - sprite_w) / 2.0f,
                    (float)bar->y + (slot_height - sprite_h) / 2.0f,
                    sprite_w,
                    sprite_h
                };
                
                DrawTexturePro(sprite_texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

        // Cooldown overlay
        if (dash_cooldown_frames > 0) {
            float cooldown_ratio = (float)dash_cooldown_frames / 180.0f;
            int grey_width = (int)(item_w * cooldown_ratio);
            if (grey_width > 0) {
                DrawRectangle((int)item_x, (int)item_y, grey_width, (int)item_h, (Color){80, 80, 80, 150});
            }
        }
    }
}

char* GetPortraitPath(int character_type) {
    switch (character_type) {
        case CHARACTER_CAVERMAN:
            return "assets/hud/stock-icon/caverman-stock.png";
        case CHARACTER_CAVERMAN2:
            return "assets/hud/stock-icon/caverman2-stock.png";
        case CHARACTER_GIRL:
            return "assets/hud/stock-icon/girl-stock.png";
        case CHARACTER_GIRL2:
            return "assets/hud/stock-icon/girl2-stock.png";
        case CHARACTER_EGG_SHELL:
            return "assets/hud/stock-icon/egg-shell-stock.png";
        case CHARACTER_EGG_SHELL_2:
            return "assets/hud/stock-icon/egg-shell2-stock.png";
        case CHARACTER_LION:
            return "assets/hud/stock-icon/lion-stock.png";
        case CHARACTER_LION2:
            return "assets/hud/stock-icon/lion2-stock.png";
        default:
            return "assets/hud/stock-icon/caverman-stock.png";
    }
}

void HUD_UnloadInventoryBar(InventoryBar* bar) {
    if (!bar) return;
    if (bar->left_texture.id != 0) UnloadTexture(bar->left_texture);
    if (bar->right_texture.id != 0) UnloadTexture(bar->right_texture);
    if (bar->middle_texture.id != 0) UnloadTexture(bar->middle_texture);
    if (bar->item_texture.id != 0) UnloadTexture(bar->item_texture);
}

void HUD_DrawStock(int stock, int x, int y, Texture2D stock_texture, bool isP2) {
    if (stock_texture.id != 0) {
        for (int i = 0; i < stock; i++) {
            int decalage = (int)(stock_texture.width * 0.15f + 10) * i;
            if (isP2) decalage = -decalage;
            
            Rectangle source = {0, 0, (float)stock_texture.width, (float)stock_texture.height};
            // flip horizontal
            if (isP2) source.width = -source.width;
            
            Rectangle dest = {(float)(x + decalage), (float)y, stock_texture.width * 0.15f, stock_texture.height * 0.15f};
            
            DrawTexturePro(stock_texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        }
    }
}




