#include "map_metadata.h"

static MapMetadata mapDatabase[MAP_COUNT] = {
    [MAP_VALLEY] = {
        .type = MAP_VALLEY,
        .playerSpawns = {
            // coordonnées (x, y)
            { 240, 320 }, 
            { 1000, 275 }  
        },
        .platformCount = 9,
        .platforms = {
            // sol
            { .x = 384, .y = 562, .width = 512, .height = 20 },
            // plateforme de gauche
            { .x = 96, .y = 384, .width = 304, .height = 5 },
            // plateforme de droite
            { .x = 880, .y = 336, .width = 304, .height = 5 },
            // sol haut
            { .x = 288, .y = 160, .width = 816, .height = 5 },
            // Maison du Bas
            { .x = 692, .y = 432, .width = 176, .height = 5 },
            // Bunker
            { .x = 132, .y = 304, .width = 180, .height = 5 },
            // grande passerelle basse
            { .x = 544, .y = 320, .width = 160, .height = 5 },
            // passerelle haute
            { .x = 768, .y = 240, .width = 160, .height = 5 },
            // mini passerelle
            { .x = 704, .y = 272, .width = 64, .height = 5 }
        }
    },
    [MAP_CAVE] = {
        .type = MAP_CAVE,
        .playerSpawns = {
            // coordonnées (x, y)
            { 195, 350 }, 
            { 1038, 350 }  
        },
        .platformCount = 4,
        .platforms = {
            // sol
            { .x = 320, .y = 542, .width = 640, .height = 20 },
            // plateforme haute
            { .x = 355, .y = 270, .width = 536, .height = 10 },
            // Plateforme gauche 
            { .x = 84, .y = 415, .width = 264, .height = 10 },
            // Plateforme droite 
            { .x = 917, .y = 415, .width = 294, .height = 10 }
        }
    },
    [MAP_ICE] = {
        .type = MAP_ICE,
        .playerSpawns = {
            // coordonnées (x, y)
            { 137, 450 }, 
            { 1094, 450 }  
        },
        .platformCount = 4,
        .platforms = {
            // sol
            { .x = 160, .y = 640, .width = 960, .height = 20 },
            // plateforme haute
            { .x = 160, .y = 352, .width = 960, .height = 10 },
            // Plateforme gauche 
            { .x = 0, .y = 512, .width = 320, .height = 10 },
            // Plateforme droite 
            { .x = 960, .y = 512, .width = 320, .height = 10 }
        }
    },
    [MAP_SURVIVAL] = {
      .type = MAP_SURVIVAL,
      .playerSpawns = {
          // coordonnées (x, y)
          { 500, 450 },
          { 0, 0 }, 
      },
      .platformCount = 1,
      .platforms = {
          // sol
          { .x = 0, .y = 624, .width = 1280, .height = 20 }
      }
    }
};

MapMetadata* GetMapMetadata(MapType mapType) {
    if (mapType >= 0 && mapType < MAP_COUNT) {
        return &mapDatabase[mapType];
    }
    return &mapDatabase[0];
}