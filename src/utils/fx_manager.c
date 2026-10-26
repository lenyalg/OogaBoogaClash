#include "fx_manager.h"
#include <stddef.h>
#include <stdio.h>

void InitFXManager(FXManager *manager) {
    if (manager == NULL) return;
    
    // Reset instances
    for (int i = 0; i < MAX_HIT_FX; i++) manager->hitInstances[i] = (FXInstance){0};
    for (int i = 0; i < MAX_DASH_FX; i++) manager->dashInstances[i] = (FXInstance){0};
    
    // Charge textures Hit
    manager->hitTextureRight = LoadTexture("assets/FX/effects/1.png");
    manager->hitTextureLeft = LoadTexture("assets/FX/effects/1_bis.png");
    if (manager->hitTextureRight.id != 0) {
        manager->hitFrameWidth = (float)manager->hitTextureRight.width / HIT_FRAME_COUNT;
        manager->hitFrameHeight = (float)manager->hitTextureRight.height;
    }
    
    // Charge textures Dash
    manager->dashTextureRight = LoadTexture("assets/FX/effects/4.png");
    manager->dashTextureLeft = LoadTexture("assets/FX/effects/4_bis.png");
    if (manager->dashTextureRight.id != 0) {
        manager->dashFrameWidth = (float)manager->dashTextureRight.width / DASH_FRAME_COUNT;
        manager->dashFrameHeight = (float)manager->dashTextureRight.height;
    }
}

void UnloadFXManager(FXManager *manager) {
    if (manager == NULL) return;
    
    if (manager->hitTextureRight.id != 0) UnloadTexture(manager->hitTextureRight);
    if (manager->hitTextureLeft.id != 0) UnloadTexture(manager->hitTextureLeft);
    if (manager->dashTextureRight.id != 0) UnloadTexture(manager->dashTextureRight);
    if (manager->dashTextureLeft.id != 0) UnloadTexture(manager->dashTextureLeft);
}

// Spawn effet de frappe
void SpawnHitFX(FXManager *manager, Vector2 position, bool facingRight) {
    if (manager == NULL || manager->hitTextureRight.id == 0) return;
    
    for (int i = 0; i < MAX_HIT_FX; i++) {
        if (!manager->hitInstances[i].active) {
            float offsetX = facingRight ? HIT_OFFSET_X : -HIT_OFFSET_X;
            manager->hitInstances[i].position = (Vector2){ position.x + offsetX, position.y + HIT_OFFSET_Y };
            manager->hitInstances[i].currentFrame = 0;
            manager->hitInstances[i].framesCounter = 0;
            manager->hitInstances[i].active = true;
            manager->hitInstances[i].facingRight = facingRight;
            return;
        }
    }
}

// Spawn effet de dash
void SpawnDashFX(FXManager *manager, Vector2 position, bool facingRight) {
    if (manager == NULL || manager->dashTextureRight.id == 0) return;
    
    for (int i = 0; i < MAX_DASH_FX; i++) {
        if (!manager->dashInstances[i].active) {
            float offsetX = facingRight ? DASH_OFFSET_X : -DASH_OFFSET_X;
            manager->dashInstances[i].position = (Vector2){ position.x + offsetX, position.y + DASH_OFFSET_Y };
            manager->dashInstances[i].currentFrame = 0;
            manager->dashInstances[i].framesCounter = 0;
            manager->dashInstances[i].active = true;
            manager->dashInstances[i].facingRight = facingRight;
            return;
        }
    }
}

// Helper: MAJ d'un tableau d'instances
static void UpdateInstances(FXInstance *instances, int count, int speed, int frameCount) {
    int threshold = 60 / speed;
    if (threshold < 1) threshold = 1;
    
    for (int i = 0; i < count; i++) {
        FXInstance *fx = &instances[i];
        if (!fx->active) continue;
        
        fx->framesCounter++;
        if (fx->framesCounter >= threshold) {
            fx->framesCounter = 0;
            fx->currentFrame++;
            if (fx->currentFrame >= frameCount) fx->active = false;
        }
    }
}

void UpdateFXManager(FXManager *manager) {
    if (manager == NULL) return;
    UpdateInstances(manager->hitInstances, MAX_HIT_FX, HIT_ANIM_SPEED, HIT_FRAME_COUNT);
    UpdateInstances(manager->dashInstances, MAX_DASH_FX, DASH_ANIM_SPEED, DASH_FRAME_COUNT);
}

// Helper: Dessin d'un tableau d'instances
static void DrawInstances(FXInstance *instances, int count, Texture2D texRight, Texture2D texLeft,
                          float frameWidth, float frameHeight, int frameCount) {
    for (int i = 0; i < count; i++) {
        FXInstance *fx = &instances[i];
        if (!fx->active) continue;
        
        // Choix texture + frame selon direction 
        Texture2D tex = fx->facingRight ? texRight : texLeft;
        int frame = fx->facingRight ? fx->currentFrame : (frameCount - 1 - fx->currentFrame);
        
        Rectangle src = { frame * frameWidth, 0, frameWidth, frameHeight };
        Rectangle dst = {
            fx->position.x - frameWidth / 2.0f,
            fx->position.y - frameHeight / 2.0f,
            frameWidth,
            frameHeight
        };
        DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
    }
}

void DrawFXManager(FXManager *manager) {
    if (manager == NULL) return;
    
    // Dash FX (derrière)
    if (manager->dashTextureRight.id != 0) {
        DrawInstances(manager->dashInstances, MAX_DASH_FX,
                      manager->dashTextureRight, manager->dashTextureLeft,
                      manager->dashFrameWidth, manager->dashFrameHeight, DASH_FRAME_COUNT);
    }
    
    // Hit FX (devant)
    if (manager->hitTextureRight.id != 0) {
        DrawInstances(manager->hitInstances, MAX_HIT_FX,
                      manager->hitTextureRight, manager->hitTextureLeft,
                      manager->hitFrameWidth, manager->hitFrameHeight, HIT_FRAME_COUNT);
    }
}
