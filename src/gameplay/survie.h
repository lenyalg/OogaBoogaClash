#ifndef SURVIE_H
#define SURVIE_H

#include "common.h"

typedef struct {
  Rectangle* meteoriteActive;
  bool* hasHit;
  int meteoriteCount;
  int maxMeteorites;
  float spawnInterval;
  float meteoriteTimer;
  int indexdebut;
  int indexfin;
  Texture2D sprite;
} MeteoriteData;

void InitMeteoriteData(MeteoriteData* data, int maxMeteorites, float spawnInterval);
void meteoriteAdd(MeteoriteData* data);
void meteoriteHandler(MeteoriteData* data, float dt);
void meteoriteDraw(MeteoriteData* data);
void meteoriteFree(MeteoriteData* data);
void CheckMeteoriteCollisions(Character* player, MeteoriteData* data);

#endif