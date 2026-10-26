#ifndef MAP_METADATA_H
#define MAP_METADATA_H

#include "raylib.h"

#define MAX_PLATFORMS_PER_MAP 10

typedef enum {
    MAP_CAVE,
    MAP_ICE,
    MAP_VALLEY,
    MAP_SURVIVAL,
    MAP_COUNT,
} MapType;

typedef struct {
    MapType type;
    Vector2 playerSpawns[2]; // [0] pour P1, [1] pour P2
    int platformCount;
    Rectangle platforms[MAX_PLATFORMS_PER_MAP];
} MapMetadata;

MapMetadata* GetMapMetadata(MapType mapType);

#endif 