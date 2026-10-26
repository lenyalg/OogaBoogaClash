#ifndef DEBUG_H
#define DEBUG_H

#include "raylib.h"
#include "common.h"
#include <stddef.h>

typedef struct {
    bool enabled;
    bool showPlayerInfo;
    bool showCollisionBoxes;
    bool showSpriteBoxes;
    bool showFPS;
    bool frameByFrame;
} DebugInfo;

// Initialiser le système de debug
void InitDebug(DebugInfo* debug);

// Toggle le debug avec une touche
void UpdateDebugInput(DebugInfo* debug);

// Afficher les informations de debug d'un joueur
void DrawPlayerDebugInfo(Character* player, float groundY, int playerNum, int yOffset, float platformY);

// Afficher les boîtes de collision
void DrawPlayerCollisionBoxes(Character* player, Color outlineColor, Color spriteBoxColor, DebugInfo* debug);

// Afficher les FPS
void DrawDebugFPS(void);

// Dessiner toutes les infos de debug
void DrawDebugOverlay(DebugInfo* debug, Character* player1, Character* player2, float groundY, float platformY);

// Retourne true si la frame doit être simulée (frame-by-frame actif => nécessite T)
bool DebugShouldStep(DebugInfo* debug);

#endif
