#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "character_metadata.h"
#include "animation.h"
#include "input.h"
#include "attack.h"
#include "map_metadata.h"
#include "fx_manager.h"

void InitPlayer(Character *player, float startX, float startY, int characterType, bool isP2);
void DealDamage(Character* attacker, Character* victim, MapMetadata* currentMap, FXManager* fxManager);
void ApplyDamage(Character* victim, float damage);
bool IsPlayerDead(Character* player);
void OnPlayerDeath(Character* player, MapMetadata* currentMap);
void MovePlayer(Character* player, float gravity, FXManager* fxManager);
void attackClassic(Character* player);

#endif