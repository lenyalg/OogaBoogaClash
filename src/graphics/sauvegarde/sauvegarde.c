#include "sauvegarde.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#define FICHIERSAUV "save.txt"

void InitSauvegarde(SauvegardeData* save) {
    save ->meilleurscoresolo = 0;
    save->nbtotparties = 0;
    save->player1Win = 0;
    save->player2Win = 0;
    save->tempsjeu = 0.0f;
}

void LoadSauvegarde(SauvegardeData* save) {
    FILE* file = fopen(FICHIERSAUV, "r");
    if (!file) {
        InitSauvegarde(save);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "meilleurscoresolo=%d", &save->meilleurscoresolo) == 1) continue;
        if (sscanf(line, "totalGames=%d", &save->nbtotparties) == 1) continue;
        if (sscanf(line, "player1Wins=%d", &save->player1Win) == 1) continue;
        if (sscanf(line, "player2Wins=%d", &save->player2Win) == 1) continue;
        if (sscanf(line, "totalTime=%f", &save->tempsjeu) == 1) continue;
    }

    fclose(file);
}

void SauvegardeGame(const SauvegardeData* save) {
    FILE* file = fopen(FICHIERSAUV, "w");
    if (!file) return;
    fprintf(file, "meilleurscoresolo=%d\n", save->meilleurscoresolo);
    fprintf(file, "totalGames=%d\n", save->nbtotparties);
    fprintf(file, "player1Wins=%d\n", save->player1Win);
    fprintf(file, "player2Wins=%d\n", save->player2Win);
    fprintf(file, "totalTime=%.2f\n", save->tempsjeu);

    fclose(file);
}

void DrawSauvegarde(const SauvegardeData* save) {
    DrawText(TextFormat("Meilleur score solo: %d", save->meilleurscoresolo), 120, 410, 30, WHITE);
    DrawText(TextFormat("Parties jouees: %d", save->nbtotparties), 120, 460, 30, WHITE);
    DrawText(TextFormat("Victoires J1: %d", save->player1Win), 120, 510, 30, WHITE);
    DrawText(TextFormat("Victoires J2: %d", save->player2Win), 120, 560, 30, WHITE);
    DrawText(TextFormat("Temps de jeu total: %.1f", save->tempsjeu), 120, 610, 30, WHITE);
}
