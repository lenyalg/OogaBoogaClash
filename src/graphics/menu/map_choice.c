#include "map_choice.h"
#include "raylib.h"
#include <stdlib.h>
#include <options.h>

static const char *mapNamesArr[] = {
    "MAP 1 - CAVE",
    "MAP 2 - ICE",
    "MAP 3 - VALLEY"
};

static const char *mapPathsArr[] = {
    "assets/Backgrounds/cave.png",
    "assets/Backgrounds/ice.png",
    "assets/Backgrounds/valley.png"
};

#define TOTAL_MAPS 3

static Texture2D Background;

void InitMapSelect(MapSelectState *state) {
    Background = LoadTexture("assets/menu/Wood_Background.png");
    state->currentMap = 0;
    state->totalMaps = TOTAL_MAPS;
    state->mapNames = mapNamesArr;

    // Charger les textures
    state->mapTextures = malloc(sizeof(Texture2D) * TOTAL_MAPS);

    for (int i = 0; i < TOTAL_MAPS; i++) {
        state->mapTextures[i] = LoadTexture(mapPathsArr[i]);
    }
}

void UpdateMapSelect(MapSelectState *state) {
    if (IsKeyPressed(KEY_LEFT)) {
        state->currentMap--;
        if (state->currentMap < 0) state->currentMap = state->totalMaps - 1;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        state->currentMap++;
        if (state->currentMap >= state->totalMaps) state->currentMap = 0;
    }
}

void DrawMapSelect(MapSelectState *state) {
    DrawTexture(Background, 0, 0, WHITE);
    DrawRectangle(0, 0, 1280, 720, (Color){0, 0, 0,120}); 

    // Titre
    const char *title = "CHOISISSEZ UNE MAP";
    int tw = MeasureText(title, 40);
    DrawText(title, (1280 - tw) / 2, 40, 40, WHITE);

    // Sprite de la map sélectionnée
    Texture2D tex = state->mapTextures[state->currentMap];
    DrawTextureEx(tex, (Vector2){320, 150}, 0, 0.5f, WHITE);

    // Nom de la map
    const char *name = state->mapNames[state->currentMap];
    DrawText(name, (1280 - 200) / 2, 550, 30, YELLOW);

    // Flèches de navigation
    DrawText("<", 250, 300, 80, LIGHTGRAY);
    DrawText(">", 1000, 300, 80, LIGHTGRAY);

    // Instructions
    DrawText("ENTER pour valider", 530, 630, 20, GREEN);
    DrawText("ESPACE pour retour au menu", 500, 660, 20, LIGHTGRAY);
}

int GetSelectedMap(MapSelectState *state) {
    if (IsKeyPressed(KEY_ENTER)) {
        return state->currentMap;
    }
    return -1; // rien sélectionné
}

void UnloadMapSelect(MapSelectState *state) {
    for (int i = 0; i < state->totalMaps; i++) {
        UnloadTexture(state->mapTextures[i]);
    }
    free(state->mapTextures);
    UnloadTexture(Background);
}
const char *GetMapPath(int index) {
    if (index < 0 || index >= TOTAL_MAPS) return mapPathsArr[0];
    return mapPathsArr[index];
}