#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"
#include "common.h"


void InitPlayerInput(PlayerInput* input, InputType type, int gamepadId);
void UpdateGamepadConnections(PlayerInput* player1Input, PlayerInput* player2Input);
bool IsActionPressed(Character* player, InputAction action);
bool IsActionDown(Character* player, InputAction action);
bool IsDownFlicked(Character* player);
int GetDashDirection(Character* player);

#endif