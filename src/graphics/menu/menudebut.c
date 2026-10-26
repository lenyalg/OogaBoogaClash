#include "menudebut.h"
#include "options.h"

// --- Boutons du menu ---
static Texture2D background;
static Texture2D scorebackground;
static Button btnPlaySolo;
static Button btnPlayCombat;
static Button btnOptions;
static Button btnExit;

bool ButtonUpdate(Button *btn, Vector2 mousePos) {
    btn->hovered = CheckCollisionPointRec(mousePos, btn->bounds);
    return (btn->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

void ButtonRender(Button *btn) {
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

void InitMenu(void)
{
    background = LoadTexture("assets/menu/Menu_Background.png");

    
    scorebackground = LoadTexture("assets/menu/score_Background.png");
    
    btnPlaySolo  = (Button){ (Rectangle){1280*0.50, 720*0.60, 1280*0.20, 720*0.1}, "SOLO", false, LoadTexture("assets/menu/Solo.png"), 1.0f };
    btnPlayCombat= (Button){ (Rectangle){1280*0.75, 720*0.60, 1280*0.20, 720*0.1}, "COMBAT", false, LoadTexture("assets/menu/Combat.png"), 1.0f };
    btnOptions   = (Button){ (Rectangle){1280*0.50, 720*0.75, 1280*0.20, 720*0.1}, "OPTIONS", false, LoadTexture("assets/menu/Options.png"), 1.0f };
    btnExit      = (Button){ (Rectangle){1280*0.75, 720*0.75, 1280*0.20, 720*0.1}, "QUITTER", false, LoadTexture("assets/menu/Quitter.png"), 1.0f };
}

void UpdateMenu(GameState *state)
{
    Vector2 mouse = GetMousePosition();

    if (ButtonUpdate(&btnPlaySolo, mouse)) *state = STATE_SURVIVAL;
    if (ButtonUpdate(&btnPlayCombat, mouse)) *state = STATE_CHARACTER_SELECT;
    if (ButtonUpdate(&btnOptions, mouse)) *state = STATE_OPTIONS;
    if (ButtonUpdate(&btnExit, mouse))    *state = STATE_EXIT;
}

void DrawMenu()
{
    DrawTexture(background, 0, 0, WHITE);
    DrawTexturePro(scorebackground,(Rectangle){0, 0, scorebackground.width, scorebackground.height},(Rectangle){0, 300, 600, 450},(Vector2){0, 0},0.0f,WHITE);
    ButtonRender(&btnPlaySolo);
    ButtonRender(&btnPlayCombat);
    ButtonRender(&btnOptions);
    ButtonRender(&btnExit);
}

void UnloadMenu(void)
{
    UnloadTexture(background);
    UnloadTexture(btnPlaySolo.img);
    UnloadTexture(btnPlayCombat.img);
    UnloadTexture(btnOptions.img);
    UnloadTexture(btnExit.img);
    UnloadTexture(scorebackground);
}
