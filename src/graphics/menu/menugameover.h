#ifndef MENUGAMEOVER_H
#define MENUGAMEOVER_H

#include "raylib.h"

typedef struct {
    const char* winnerText;
    bool active;
    float timer;
    Texture2D spritewinner;
} GameOverState;

// Initialise le module Game Over
void InitGameOver(GameOverState* state);

// Met à jour le Game Over (vérifie si le joueur appuie sur SPACE)
void UpdateGameOver(GameOverState* state, int* gameState);

// Dessine le menu Game Over
void DrawGameOver(GameOverState *state, int screenWidth);

// Enleve toutes les images chargés pour le menu
void UnloadGameOver(GameOverState* state);

#endif
