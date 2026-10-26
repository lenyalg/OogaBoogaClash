#include "survie.h"
#include <stdlib.h>
#include "common.h"

void InitMeteoriteData(MeteoriteData* data, int maxMeteorites, float spawnInterval){
  if (data == NULL) {
    return;
  }
  data->meteoriteActive = malloc(sizeof(Rectangle) * maxMeteorites);
  data->hasHit = malloc(sizeof(bool) * maxMeteorites);
  if (data->meteoriteActive == NULL || data->hasHit == NULL) {
    return;
  }

  for (int i = 0; i < maxMeteorites; i++) {
    data->hasHit[i] = false;
  }

  data->meteoriteCount = 0;
  data->maxMeteorites = maxMeteorites;
  data->spawnInterval = spawnInterval;
  data->meteoriteTimer = 0.0f;
  data->indexdebut = 0;
  data->indexfin = 0;
  data->sprite = LoadTexture("assets/items/meteorite.png");
}

void meteoriteAdd(MeteoriteData* data){
  if (data == NULL || data->meteoriteActive == NULL) {
    return;
  }
  
  Rectangle hitbox = {
    .x = rand() % (WINDOWWIDTH - 50),
    .y = -WINDOWHEIGHT - rand() % 200,
    .width = 50,
    .height = 75
  };

  int index = data->indexfin % data->maxMeteorites;
  data->meteoriteActive[index] = hitbox;
  data->hasHit[index] = false;
  data->indexfin++;
  
  if (data->meteoriteCount < data->maxMeteorites) {
    data->meteoriteCount++;
  } else {
    data->indexdebut++;
  }
}

void meteoriteHandler(MeteoriteData* data, float dt){
  if (data == NULL || data->meteoriteActive == NULL || data->meteoriteCount <= 0) {
    return;
  }
  
  for (int i = 0; i < data->meteoriteCount; i++)
  {
    int index = (data->indexdebut + i) % data->maxMeteorites;
    data->meteoriteActive[index].y += 300 * dt;
  }
}

void meteoriteDraw(MeteoriteData* data){
  if (data == NULL || data->meteoriteActive == NULL || data->meteoriteCount <= 0) {
    return;
  }
  
  for (int i = 0; i < data->meteoriteCount; i++)
  {
    int index = (data->indexdebut + i) % data->maxMeteorites;
    if (!data->hasHit[index])
    {
      // Dessiner la texture redimensionnée à la taille METEORITE_SIZE
      Rectangle sourceRec = {0, 0, (float)data->sprite.width, (float)data->sprite.height};
      Rectangle destRec = {
        data->meteoriteActive[index].x, 
        data->meteoriteActive[index].y, 
        data->meteoriteActive[index].width, 
        data->meteoriteActive[index].height
      };
      DrawTexturePro(data->sprite, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    }
  }
}

void CheckMeteoriteCollisions(Character* player, MeteoriteData* data){
  if (player == NULL || data == NULL || data->meteoriteActive == NULL || data->meteoriteCount <= 0) {
    return;
  }
  
  Rectangle playerRect = {
    .x = player->x,
    .y = player->y,
    .width = player->width,
    .height = player->height
  };
  
  for (int i = 0; i < data->meteoriteCount; i++)
  {
    int index = (data->indexdebut + i) % data->maxMeteorites;
    if (CheckCollisionRecs(playerRect, data->meteoriteActive[index]) && !data->hasHit[index]) {
      player->health -= player->maxHealth/3; 
      data->hasHit[index] = true;
    }
  }
}

void meteoriteFree(MeteoriteData* data){
  if (data == NULL) {
    return;
  }
  
  if (data->meteoriteActive != NULL) {
    free(data->meteoriteActive);
    data->meteoriteActive = NULL;
  }
  
  if (data->hasHit != NULL) {
    free(data->hasHit);
    data->hasHit = NULL;
  }
  
  free(data);
}