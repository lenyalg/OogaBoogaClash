#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

typedef struct {
    int meilleurscoresolo;
    int nbtotparties;
    int player1Win;
    int player2Win;
    float tempsjeu;
} SauvegardeData;

void InitSauvegarde(SauvegardeData* sauvegarde);
void LoadSauvegarde(SauvegardeData* sauvegarde);
void SauvegardeGame(const SauvegardeData* sauvegarde);
void DrawSauvegarde(const SauvegardeData* sauvegarde);

#endif
