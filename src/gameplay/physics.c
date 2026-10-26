#include "physics.h"



void UpdatePhysics(Character *player, float dt, float gravity, GameState state) {
    if (dt > 0.1f) dt = 0.1f;
    
    player->previousY = player->y;
    player->velocityY += gravity * dt;
    if (player->velocityY > player->maxFallSpeed) player->velocityY = player->maxFallSpeed;
    player->y += player->velocityY * dt;
    player->x += player->velocityX * dt;
    player->spriteY = player->y;
    player->spriteX = player->x;

    int minX;
    int maxX;
    if (state == STATE_SURVIVAL) {
      minX = 40;
      maxX = WINDOWWIDTH-40;
    }
    else {
      minX = 0;
      maxX = WINDOWWIDTH;
    }

    if (player->x < minX) { 
      player->x = minX; 
      player->spriteX = minX; 
      player->velocityX = 0; 
    }
    if (player->x + player->width >= maxX) { 
      player->x = maxX - player->width; 
      player->spriteX = maxX - player->width; 
      player->velocityX = 0;
    }
            
}

