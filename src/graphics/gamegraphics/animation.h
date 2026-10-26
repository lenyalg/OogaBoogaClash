#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"

void UpdateAnimState(Character *player, bool isMoving);
void DrawCharacter(Character *player, Texture2D sprite, Color tint);

#endif