#ifndef ATTACK_H
#define ATTACK_H

#include "common.h"

void StartAttack(Character *player, int attackIndex);
void UpdateAttack(Character *player);
bool IsAttackHitboxActive(Character *player);
void ResetAttackHit(Character *player);
bool CheckAttackCollision(Character *attacker, Character *victim);

#endif
