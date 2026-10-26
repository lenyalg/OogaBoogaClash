#include "collision.h"

void CheckCollisionsGround(Character *player, float groundY, float groundX, float groundWidth) {
  // Sol Principal
  // On verifie si on est bien sur le sol principale
  if (player->y + player->height >= groundY && player->previousY + player->height <= groundY && player->x + player->width > groundX && player->x < groundX + groundWidth) {
    player->y = groundY - player->height;
    player->spriteY = player->y;
    player->velocityY = 0.0f;
    player->isOnGround = true;
    player->isFastFalling = false;
    player->hasDoubleJumped = true;
    player->standingPlatformId = -1;
    player->dropThroughPlatform = false;
  } 
  // On verifie si on était bien sur le sol principale mais 
  // si on est a l'exterieur du sol alors on met onGround a false pour enlever le 2eme saut
  // dans le cas où on drop du sol juste en se laissant tomber dans le vide
  else if (player->isOnGround && player->standingPlatformId == -1 && 
             (!(player->x + player->width > groundX && player->x < groundX + groundWidth) || 
              player->y + player->height < groundY || 
              player->y + player->height > groundY + 20)) {
    player->isOnGround = false;
  }
}

// Fonction pour savoir si on est tomber dans le vide et gerer le respawn
void CheckVoid(Character* player, MapMetadata* currentMap){
  if (player->y >= WINDOWHEIGHT+100)
  {
    player->y = currentMap->platforms[0].y - player->height-50;
    player->spriteY = currentMap->platforms[0].y - player->height-50;
    player->x = currentMap->platforms[0].x + currentMap->platforms[0].width /2;
    player->spriteX = currentMap->platforms[0].x + currentMap->platforms[0].width /2;
    player->health = player->maxHealth;
    player->stock -= 1;
    player->velocityY = 0.0f;
    player->velocityX = 0.0f;
  }
}

void CheckCollisionPlateform(Character *player, int platformId, float platformY, float platformX, float platformWidth, float groundPlatformY) {

  // Si on appuyer sur DOWN en étant sur une platforme et que ce n'est pas la platforme principale
  // Alors on met onGround a false pour enlever le 2eme saut si on drop d'une platforme en appuyant sur DOWN
  if (IsActionPressed(player, DOWN) && player->isOnGround && !(player->y + player->height >= groundPlatformY)) { 
    player->isOnGround = false;
  }

  // On regarde si on est sur une platforme et que la platforme sur laquelle on est est bien la
  // plateforme sur laquelle on fait la verification
  // Puis si on est a l'exterieur de cette platforme alors on met onGround a false pour enlever le 2eme saut
  // dans le cas où on drop de la platforme juste en se laissant tomber
  if (player->isOnGround && player->standingPlatformId == platformId &&
      (player->x + player->width <= platformX || player->x >= platformX + platformWidth)) {
    player->isOnGround = false;
    player->standingPlatformId = -1;
  }

  //prevBottom est la position du bas du perso a la frame d'avant
  float prevBottom = player->previousY + player->height;
  //currBottm est la position du bas du perso a la frame actuelle
  float currBottom = player->y + player->height;

  // On verifie si le perso est bien entre les 2 bords de la platforme
  bool onPlatformX = (player->x + player->width > platformX && player->x < platformX + platformWidth);
  // On verifie si le perso est passer juste en dessous de la plateforme et en meme temps si 
  // la frame d'avant on était bien au dessus
  // Cela permet de savoir si on est bien arrivé sur la platforme
  // Un == ne fonctionne pas car en utilisant les floats il est peu probable que ca arrive
  bool onPlatformY = (prevBottom <= platformY && currBottom >= platformY);

  // On regarde ici si on est bien sur la platforme avec onPlatformX && onPlatformY
  // Puis on regarde la velocité afin de pouvoir traverser la plateforme depuis le bas mais pas en venant du haut
  // Puis si on appuie sur la touche DOWN on ignore simplement la platforme
  bool onPlatform = (onPlatformX && onPlatformY && player->velocityY >= 0 && !IsActionDown(player, DOWN));

  if (onPlatform) {
    player->y = platformY - player->height;
    player->spriteY = player->y;
    player->velocityY = 0.0f;
    player->isOnGround = true;
    player->isFastFalling = false;
    player->hasDoubleJumped = true;
    player->standingPlatformId = platformId;
    player->dropThroughPlatform = false;
  }
}