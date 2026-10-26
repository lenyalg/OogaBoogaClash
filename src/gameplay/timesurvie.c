#include "timesurvie.h"
#include "raylib.h"

void Survival_Init(Timesurvival* survival)
{
    if (!survival) return;

    survival->time = 0.0f;
    survival->score = 0.0f;
    survival->scoreInt = 0;
    survival->scoreRate = 10.0f; // points par seconde
    survival->active = true;
}

void Survival_Update(Timesurvival* survival, float dt)
{
    if (!survival || !survival->active) return;

    survival->time += dt;
    survival->score += survival->scoreRate * dt * 1.1;
    survival->scoreInt = (int)(survival->score);
}

void Survival_Stop(Timesurvival* survival)
{
    if (!survival) return;

    survival->active = false;
}

void Survival_Reset(Timesurvival* survival)
{
    if (!survival) return;

    survival->time = 0.0f;
    survival->score = 0;
    survival->active = false;
}

void Survival_DrawHUD(const Timesurvival* survival)
{
    if (!survival) return;

    DrawText(TextFormat("TIME : %.1f", survival->time), 1020, 80, 20, BLACK);
    DrawText(TextFormat("SCORE : %d", survival->scoreInt), 1020, 110, 20, BLACK);
}
