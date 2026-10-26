#ifndef COLLISION_H
#define COLLISION_H

#include "common.h"
#include "player.h"
#include "input.h"
#include "map_metadata.h"

void CheckCollisionsGround(Character *player, float groundY, float groundX, float groundWidth);
void CheckCollisionPlateform(Character *player, int platformId, float platformY, float platformX, float platformWidth, float groundPlatformY);
void CheckVoid(Character* player, MapMetadata* currentMap);
#endif