#ifndef TIMESURVIE_H
#define TIMESURVIE_H

#include <stdbool.h>

typedef struct {
    float time;
    float score;
    int scoreInt;
    float scoreRate;
    bool active;
} Timesurvival;

// Initialisation
void Survival_Init(Timesurvival* survival);

void Survival_Update(Timesurvival* survival, float dt);
void Survival_Stop(Timesurvival* survival);
void Survival_Reset(Timesurvival* survival);
void Survival_DrawHUD(const Timesurvival* survival);

#endif
