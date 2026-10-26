#ifndef FX_MANAGER_H
#define FX_MANAGER_H

#include "raylib.h"
#include <stdbool.h>

// Configuration Hit FX
#define MAX_HIT_FX 16
#define HIT_FRAME_COUNT 4
#define HIT_ANIM_SPEED 20
#define HIT_OFFSET_X 16.0f
#define HIT_OFFSET_Y -15.0f

// Configuration Dash FX
#define MAX_DASH_FX 6
#define DASH_FRAME_COUNT 6
#define DASH_ANIM_SPEED 18
#define DASH_OFFSET_X 0.0f
#define DASH_OFFSET_Y 0.0f

// Instance d'un effet visuel 
typedef struct {
    Vector2 position;
    int currentFrame;
    int framesCounter;
    bool active;
    bool facingRight;
} FXInstance;

// Gestionnaire des effets visuels
typedef struct {
    // Hit FX
    FXInstance hitInstances[MAX_HIT_FX];
    Texture2D hitTextureRight;    
    Texture2D hitTextureLeft;       
    float hitFrameWidth;
    float hitFrameHeight;
    
    // Dash FX
    FXInstance dashInstances[MAX_DASH_FX];
    Texture2D dashTextureRight; 
    Texture2D dashTextureLeft;    
    float dashFrameWidth;
    float dashFrameHeight;
} FXManager;

void InitFXManager(FXManager *manager);
void UnloadFXManager(FXManager *manager);
void SpawnHitFX(FXManager *manager, Vector2 position, bool facingRight);
void SpawnDashFX(FXManager *manager, Vector2 position, bool facingRight);
void UpdateFXManager(FXManager *manager);
void DrawFXManager(FXManager *manager);

#endif
