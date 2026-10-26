#include "attack.h"
#include "player.h"


void StartAttack(Character *player, int attackIndex) {
  if (player->isAttacking) return;
  player->isAttacking = true;
  player->currentAttack.currentSpriteIndex = 0;
  player->currentAttack.framesInCurrentSprite = 0;
  int currentIndex = 0;

  while (attackIndex != player->attacks[currentIndex].attackIndex && currentIndex < player->attackCount) {
    currentIndex++;
  }

  if (currentIndex < player->attackCount) {
    player->currentAttack.attackData = &player->attacks[currentIndex];
  }

  player->hasHitThisAttack = false;
}

void UpdateAttack(Character *player) {
    if (!player->isAttacking) return;
    
    AttackAnimationData *currentAttack = &player->currentAttack;
    currentAttack->framesInCurrentSprite++;

    ActionSpriteData currentSprite = currentAttack->attackData->spriteData[currentAttack->currentSpriteIndex];
    if (currentAttack->framesInCurrentSprite >= currentSprite.duration) {
      
        currentAttack->framesInCurrentSprite = 0;
        currentAttack->currentSpriteIndex++;

        if (currentAttack->currentSpriteIndex >= currentAttack->attackData->spriteCount) {
            player->isAttacking = false;
            ResetAttackHit(player);
            currentAttack->currentSpriteIndex = 0;
        }
    }
}

bool IsAttackHitboxActive(Character *player) {
    if (!player->isAttacking) return false;
    
    int totalFramesPassed = 0;
    for (int i = 0; i < player->currentAttack.currentSpriteIndex; i++) {
        totalFramesPassed += player->currentAttack.attackData->spriteData[i].duration;
    }
    totalFramesPassed += player->currentAttack.framesInCurrentSprite;
    
    return (totalFramesPassed >= player->currentAttack.attackData->startupFrames) && 
           (totalFramesPassed < player->currentAttack.attackData->startupFrames + player->currentAttack.attackData->activeFrames);
}

void ResetAttackHit(Character *player) {
    player->hasHitThisAttack = false;
}

bool CheckAttackCollision(Character *attacker, Character *victim) {
    if (!IsAttackHitboxActive(attacker) || attacker->hasHitThisAttack) {
        return false;
    }

    float hitboxX;
    if (attacker->isFacingRight) {
      hitboxX = attacker->x + attacker->width/2 + attacker->currentAttack.attackData->hitboxOffsetX;
    } else {
      hitboxX = attacker->x + attacker->width/2 - attacker->currentAttack.attackData->hitboxOffsetX - attacker->currentAttack.attackData->hitboxWidth;
    }

    Rectangle attackHitbox = {
        hitboxX,
        attacker->y + attacker->currentAttack.attackData->hitboxOffsetY,
        attacker->currentAttack.attackData->hitboxWidth,
        attacker->currentAttack.attackData->hitboxHeight
    };
    Rectangle victimHurtbox = (Rectangle){victim->x, victim->y, victim->width, victim->height};
    
    if (CheckCollisionRecs(attackHitbox, victimHurtbox)) {
        return true;
    }
    
    return false;
}
