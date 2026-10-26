#include "player.h"
#include "sound_manager.h"

void InitPlayer(Character *player, float X, float Y, int characterType, bool isP2) {
    Character metadata = GetCharacterMetadata(characterType);
    player->x = X;
    player->y = Y;
    player->spriteX = X;
    player->spriteY = Y;
    player->previousY = Y;
    player->standingPlatformId = -1;
    player->width = metadata.width;
    player->height = metadata.height;
    player->weight = metadata.weight;
    player->speed = metadata.speed;
    player->health = metadata.health;
    player->velocityY = 0.0f;
    player->velocityX = 0.0f;
    player->isOnGround = false;
    player->jumpImpulse = metadata.jumpImpulse;
    player->maxFallSpeed = 1200.0f;
    player->isFastFalling = false;
    player->yBeforeJump = 0.0f;
    player->dropThroughPlatform = false;
    player->hasDoubleJumped = true;
    player->fastFallImpulse = 600.0f;
    player->characterType = characterType;
    player->isP2 = isP2;
    player->stock = 3;
    player->maxHealth = metadata.health;
    
    player->currentFrame = 0;
    player->framesCounter = 0;
    player->isFacingRight = !isP2;
    player->state = ANIM_IDLE;
    player->previousState = ANIM_IDLE;
    player->spriteMetadata = GetSpriteMetadataPtr(player->characterType);
    
    // Combat init
    player->isAttacking = false;
    player->hasHitThisAttack = false;
    player->attackCount = metadata.attackCount;
    player->attacks = metadata.attacks;
    player->isHited = false;
    player->timerHitstun = 0.0f;
    player->isDashing = false;
    player->dashFramesRemaining = 0;
    player->dashDurationFrames = 40;
    player->dashCooldownFramesRemaining = 0;
    player->dashCooldownFrames = 180; // 3 secondes (à 60 FPS)
    player->dashDirection = 0;
    player->dashSpeed = 10;
}


void DealDamage(Character* attacker, Character* victim, MapMetadata* currentMap, FXManager* fxManager) {
  if (!IsAttackHitboxActive(attacker) || attacker->hasHitThisAttack || attacker->isAttacking == false) {
    return;
  }

  // Spawn de l'effet visuel au centre de la victime (direction = celle de l'attaquant)
  Vector2 hitPos = { victim->x + victim->width / 2, victim->y + victim->height / 2 };
  SpawnHitFX(fxManager, hitPos, attacker->isFacingRight);
  
  PlayHitSound();
  
  victim->isHited = true;
  // Toutes les valeurs doivent varié selon l'attaque utilisée normalement et donc doivent etre stocké dans l'attaque
  victim->timerHitstun = attacker->currentAttack.attackData->timeStun;
  victim->velocityY -= attacker->currentAttack.attackData->verticalKnockback; // Knockback vertical
  // Knockback horizontal 
  if (attacker->x < victim->x) {
    victim->velocityX = attacker->currentAttack.attackData->horizontalKnockback; // Knockback droite
  } else {
    victim->velocityX = -attacker->currentAttack.attackData->horizontalKnockback; // Knockback gauche
  }

  ApplyDamage(victim, attacker->currentAttack.attackData->damage);
  attacker->hasHitThisAttack = true;
  if (IsPlayerDead(victim))
  {
    OnPlayerDeath(victim, currentMap);
  }
}

void ApplyDamage(Character* victim, float damage) {
    victim->health -= damage;
    if (victim->health < 0) {
        victim->health = 0;
    }
}

bool IsPlayerDead(Character* player) {
    return player->health <= 0;
}

void OnPlayerDeath(Character* player, MapMetadata* currentMap) {
    player->y = currentMap->platforms[0].y - player->height-50;
    player->spriteY = currentMap->platforms[0].y - player->height-50;
    player->x = currentMap->platforms[0].x + currentMap->platforms[0].width /2;
    player->spriteX = currentMap->platforms[0].x + currentMap->platforms[0].width /2;
    player->health = player->maxHealth;
    player->stock -= 1;
    player->velocityY = 0.0f;
    player->velocityX = 0.0f;
}

void HorizontalMovement(Character* player, FXManager* fxManager) {
    int dashDir = GetDashDirection(player);
    if (dashDir != 0 && !player->isDashing && player->dashCooldownFramesRemaining == 0 && player->dashFramesRemaining == 0) {
        player->isDashing = true;
        player->dashDirection = dashDir;
        player->dashFramesRemaining = player->dashDurationFrames;
        player->dashCooldownFramesRemaining = player->dashCooldownFrames;
        
        // Spawn effet de dash au centre du joueur
        Vector2 dashPos = { player->x + player->width / 2, player->y + player->height / 2 };
        SpawnDashFX(fxManager, dashPos, dashDir > 0);
    }

    if (player->dashFramesRemaining > 0) {
        if ((IsActionDown(player, LEFT) && player->dashDirection > 0) || (IsActionDown(player, RIGHT) && player->dashDirection < 0)) {
            player->dashFramesRemaining = 0;
            player->isDashing = false;
        } else {
            player->x += player->dashDirection * player->dashSpeed;
            player->spriteX += player->dashDirection * player->dashSpeed;
            player->dashFramesRemaining--;
            player->moving = true;
            player->isFacingRight = (player->dashDirection > 0);
            if (player->x < 0) { player->x = 0; player->spriteX = 0; }
            if (player->x + player->width >= WINDOWWIDTH) { player->x = WINDOWWIDTH - player->width; player->spriteX = WINDOWWIDTH - player->width; }
            if (player->dashFramesRemaining == 0) {
                player->isDashing = false;
            }
        }
    } else {
        if (IsActionDown(player, LEFT)) {
            player->x -= player->speed;
            player->spriteX -= player->speed;
            player->isFacingRight = false;
            player->moving = true;
            if (player->x <= 0) {
                player->x = 0;
                player->spriteX = 0;
            }
        }
        if (IsActionDown(player, RIGHT)) {
            player->x += player->speed;
            player->spriteX += player->speed;
            player->isFacingRight = true;
            player->moving = true;
            if (player->x + player->width >= WINDOWWIDTH) {
                player->x = WINDOWWIDTH - player->width;
                player->spriteX = WINDOWWIDTH - player->width;
            }
        }
    }

    // Décrémenter le cooldown du dash si nécessaire
    if (player->dashCooldownFramesRemaining > 0) {
        player->dashCooldownFramesRemaining--;
        if (player->dashCooldownFramesRemaining < 0) player->dashCooldownFramesRemaining = 0;
    }
    if (!IsActionDown(player, LEFT) && !IsActionDown(player, RIGHT)) {
        player->moving = false;
    }
}

void VerticalMovement(Character* player, float gravity) {
    bool downFlicked = IsDownFlicked(player);
    
    bool canDoubleJump1 = player->hasDoubleJumped && !player->isOnGround;
    if (IsActionPressed(player, JUMP) && (player->isOnGround || canDoubleJump1)) {
      if (canDoubleJump1) player->hasDoubleJumped = false;
      player->isOnGround = false;
      player->velocityY = -player->jumpImpulse;
    }

    float maxJumpHeight = (player->jumpImpulse * player->jumpImpulse) / (2 * gravity);

    if (downFlicked && 
        !player->isOnGround && 
        !player->isFastFalling && 
        ((player->y >= player->yBeforeJump - maxJumpHeight && player->velocityY > 0) ||
         (player->y <= player->yBeforeJump - maxJumpHeight + 8)))
    {
      player->velocityY += player->fastFallImpulse;
      player->isFastFalling = true;
    }
}
    
void MovePlayer(Character* player, float gravity, FXManager* fxManager) {
  if (!player->isHited)
  {
    HorizontalMovement(player, fxManager);
    VerticalMovement(player, gravity);
  }
}


void attackClassic(Character* player){
    if (IsActionPressed(player, ATTACK) && !player->isAttacking && !player->isHited) {
      // const ActionSpriteData attackSequence[] = {
      //   {0, 3},  // Sprite 0, 3 frames
      //   {1, 5},  // Sprite 1, 5 frames
      //   {2, 4}   // Sprite 2, 4 frames
      // };
      StartAttack(player, 0);
    }
}