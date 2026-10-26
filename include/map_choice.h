#ifndef MAP_CHOICE_H
#define MAP_CHOICE_H

#include "raylib.h"
#include "common.h"
#include "sprite_manager.h"

typedef struct {
    int currentMap;     
    int totalMaps;      
    Texture2D *mapTextures; 
    const char **mapNames;  
} MapSelectState;

void InitMapSelect(MapSelectState *state);
void UpdateMapSelect(MapSelectState *state);
void DrawMapSelect(MapSelectState *state);
int GetSelectedMap(MapSelectState *state);
void UnloadMapSelect(MapSelectState *state);

const char *GetMapPath(int index);

#endif