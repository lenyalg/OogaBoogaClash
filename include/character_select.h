#ifndef CHARACTER_SELECT_H
#define CHARACTER_SELECT_H

#include "raylib.h"
#include "common.h"
#include "sprite_manager.h"

typedef struct {
    int player1Selected;
    int player2Selected;
    int player1Index;  
    int player2Index;  
    Texture2D player1Portrait;
    Texture2D player2Portrait;
    int totalCharacters;
} CharacterSelectState;

void InitCharacterSelect(CharacterSelectState *state);

void UpdateCharacterSelect(CharacterSelectState *state);

void DrawCharacterSelect(CharacterSelectState *state);

const char *GetSpritePath(int characterType);

#endif
