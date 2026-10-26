#ifndef OPTIONS_H
#define OPTIONS_H

#include "raylib.h"
#include "common.h"

typedef struct {
    InputType player1InputType;
    InputType player2InputType;
    int player1GamepadId;  
    int player2GamepadId;
    int selectedOption; 
} OptionsState;

void InitOptionsState(OptionsState* options);
void UpdateOptionsMenu(OptionsState* options);
void DrawOptionsMenu(Texture2D background, OptionsState* options);

#endif
