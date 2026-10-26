#include "menugameover.h"
#include "options.h"

static Texture2D background;
static Button btnExit;

bool ButtonUpdate_GO(Button *btn, Vector2 mousePos) {
    btn->hovered = CheckCollisionPointRec(mousePos, btn->bounds);
    return (btn->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

void ButtonRender_GO(Button *btn) {
    float targetScale = btn->hovered ? 1.1f : 1.0f;
    btn->scale += (targetScale - btn->scale) * 0.2f;

    float width = btn->bounds.width * btn->scale;
    float height = btn->bounds.height * btn->scale;
    float x = btn->bounds.x - (width - btn->bounds.width) / 2;
    float y = btn->bounds.y - (height - btn->bounds.height) / 2;

    DrawTexturePro(
        btn->img,
        (Rectangle){0, 0, btn->img.width, btn->img.height},
        (Rectangle){x, y, width, height},
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

void InitGameOver(GameOverState* state)
{
    state->winnerText = "";
    state->active = false;
    state->timer = 0.0f;
    background = LoadTexture("assets/menu/Wood_Background.png");
    btnExit = (Button){ (Rectangle){1280*0.4, 720*0.85, 1280*0.20, 720*0.1},"QUITTER", false, LoadTexture("assets/menu/Quitter.png"), 1.0f };
    state->spritewinner.id = 0;
}

void UpdateGameOver(GameOverState* state, int* gameState)
{
    Vector2 mouse = GetMousePosition();

    if (*gameState != STATE_GAMEOVER) return;

    if (ButtonUpdate_GO(&btnExit, mouse))
    {
        *gameState = STATE_MENU;
        state->active = false;
    }
}

void DrawGameOver(GameOverState* state, int screenWidth)
{
    if (!state->active) return;

    DrawTexture(background, 0, 0, WHITE);
    DrawText("GAME OVER",screenWidth / 2 - MeasureText("GAME OVER", 100) / 2,120,100,BLACK);

    if(state->timer > 0.0f){
        DrawText(TextFormat("SCORE : %.0f", state->timer),screenWidth / 2 - MeasureText(TextFormat("SCORE : %.0f", state->timer), 50) / 2,500,50,BLACK);
    }

    if (state->spritewinner.id != 0){
        DrawTextureEx(
            state->spritewinner,
            (Vector2){screenWidth / 2 - 100,260},0.0f,200.0f / state->spritewinner.width,WHITE);
    }
    DrawText(state->winnerText,screenWidth / 2 - MeasureText(state->winnerText, 50) / 2,500,50,BLACK);
    ButtonRender_GO(&btnExit);
}


void UnloadGameOver(GameOverState* state)
{
    UnloadTexture(background);
    UnloadTexture(btnExit.img);
    UnloadTexture(state->spritewinner);
}