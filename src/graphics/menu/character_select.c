#include "character_select.h"
#include "sprite_manager.h"
#include <options.h>

#define TOTAL_CHARACTERS 8

typedef struct {
    int type;
    const char *name;
    const char *spritePath;
    const char *portraitPath;
} CharacterData;

static const CharacterData characterData[TOTAL_CHARACTERS] = {
    {CHARACTER_CAVERMAN, "CAVEMAN", "assets/characters/Playable/caverman.png", "assets/characters/Playable/pictures/caverman-picture.png"},
    {CHARACTER_CAVERMAN2, "CAVEMAN 2", "assets/characters/Playable/caverman-2.png", "assets/characters/Playable/pictures/caverman2-picture.png"},
    {CHARACTER_GIRL, "GIRL", "assets/characters/Playable/girl.png", "assets/characters/Playable/pictures/girl-picture.png"},
    {CHARACTER_GIRL2, "GIRL 2", "assets/characters/Playable/girl-2.png", "assets/characters/Playable/pictures/girl2-picture.png"},
    {CHARACTER_EGG_SHELL, "EGG-SHELL", "assets/characters/Playable/egg-shell.png", "assets/characters/Playable/pictures/egg-shell-picture.png"},
    {CHARACTER_EGG_SHELL_2, "EGG-SHELL 2", "assets/characters/Playable/egg-shell-2.png", "assets/characters/Playable/pictures/egg-shell2-picture.png"},
    {CHARACTER_LION, "LION", "assets/characters/Playable/lion.png", "assets/characters/Playable/pictures/lion-picture.png"},
    {CHARACTER_LION2, "LION 2", "assets/characters/Playable/lion-2.png", "assets/characters/Playable/pictures/lion2-picture.png"}
};

static const char *DEFAULT_SPRITE_PATH = "assets/characters/Playable/caverman.png";

static Texture2D background;

void InitCharacterSelect(CharacterSelectState *state) {
    background = LoadTexture("assets/menu/Wood_Background.png");
    state->player1Selected = characterData[0].type;
    state->player2Selected = characterData[2].type;
    state->player1Index = 0;
    state->player2Index = 2;
    state->totalCharacters = TOTAL_CHARACTERS;
    state->player1Portrait = LoadTexture(characterData[state->player1Index].portraitPath);
    state->player2Portrait = LoadTexture(characterData[state->player2Index].portraitPath);
}

void UpdateCharacterSelect(CharacterSelectState *state) {
    // Joueur 1 :Navigation skins
    if (IsKeyPressed(KEY_A)) {
        state->player1Index--;
        if (state->player1Index < 0) {
            state->player1Index = state->totalCharacters - 1;
        }
        state->player1Selected = characterData[state->player1Index].type;
        UnloadTexture(state->player1Portrait);
        state->player1Portrait = LoadTexture(characterData[state->player1Index].portraitPath);
    }
    
    if (IsKeyPressed(KEY_D)) {
        state->player1Index++;
        if (state->player1Index >= state->totalCharacters) {
            state->player1Index = 0;
        }
        state->player1Selected = characterData[state->player1Index].type;
        UnloadTexture(state->player1Portrait);
        state->player1Portrait = LoadTexture(characterData[state->player1Index].portraitPath);
    }
    
    // Joueur 2 : Navigation skins
    if (IsKeyPressed(KEY_K)) {
        state->player2Index--;
        if (state->player2Index < 0) {
            state->player2Index = state->totalCharacters - 1;
        }
        state->player2Selected = characterData[state->player2Index].type;
        UnloadTexture(state->player2Portrait);
        state->player2Portrait = LoadTexture(characterData[state->player2Index].portraitPath);
    }
    
    if (IsKeyPressed(KEY_L)) {
        state->player2Index++;
        if (state->player2Index >= state->totalCharacters) {
            state->player2Index = 0;
        }
        state->player2Selected = characterData[state->player2Index].type;
        UnloadTexture(state->player2Portrait);
        state->player2Portrait = LoadTexture(characterData[state->player2Index].portraitPath);
    }
}

void DrawCharacterSelect(CharacterSelectState *state) {
    DrawTexture(background, 0, 0, WHITE);
    DrawRectangle(0, 0, WINDOWWIDTH, WINDOWHEIGHT, (Color){0, 0, 0, 120});
    const char *title = "VEUILLEZ CHOISIR VOTRE PERSONNAGE";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, (WINDOWWIDTH - titleWidth) / 2, 40, 50, WHITE);
    
    // Charger les portraits
    Texture2D player1Portrait = state->player1Portrait;
    Texture2D player2Portrait = state->player2Portrait;
    
    // Dimensions des portraits
    int portraitWidth = 300;
    int portraitY = 150;
    
    // Position Joueur 1 (gauche)
    int player1X = 100;
    
    // Position Joueur 2 (droite)
    int player2X = WINDOWWIDTH - portraitWidth - 100;
    
    // ========== JOUEUR 1 (GAUCHE) ==========
    DrawText("JOUEUR 1", player1X, portraitY, 35, BLUE);
    const char *player1Name = characterData[state->player1Index].name;
    DrawText(player1Name, player1X, portraitY + 50, 40, WHITE);
    
    // Afficher le portrait du joueur 1
    if (player1Portrait.id != 0) {
        DrawTexture(player1Portrait, player1X, portraitY + 120, WHITE);
    }
    
    // Touches de navigation Joueur 1
    DrawText("(<- Q / D ->)", player1X, portraitY + 320, 20, LIGHTGRAY);

    
    // ========== JOUEUR 2 (DROITE) ==========
    DrawText("JOUEUR 2", player2X, portraitY, 35, RED);
    const char *player2Name = characterData[state->player2Index].name;
    DrawText(player2Name, player2X, portraitY + 50, 40, WHITE);
    
    // Afficher le portrait du joueur 2
    if (player2Portrait.id != 0) {
        // Flip horizontal uniquement
        Rectangle source = {0, 0, -player2Portrait.width, player2Portrait.height};
        Rectangle dest = {player2X + player2Portrait.width -220, portraitY + 120, player2Portrait.width, player2Portrait.height};
        DrawTexturePro(player2Portrait, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
    }
    
    // Touches de navigation Joueur 2
    DrawText("(<- K / L ->)", player2X, portraitY + 320, 20, LIGHTGRAY);
    
    DrawText("ENTRER pour commencer", 
             (WINDOWWIDTH - MeasureText("ENTRER pour commencer", 20)) / 2, 
             WINDOWHEIGHT - 60, 20, WHITE);
    DrawText("ESPACE pour retourner au menu", 
             (WINDOWWIDTH - MeasureText("ESPACE pour retourner au menu", 20)) / 2, 
             WINDOWHEIGHT - 30, 20, LIGHTGRAY);
}

void UnloadCharacterSelect(CharacterSelectState *state) {
    UnloadTexture(state->player1Portrait);
    UnloadTexture(state->player2Portrait);
    UnloadTexture(background);
}


// utilisée dans main pour charger les sprites
const char *GetSpritePath(int characterType) {
    for (int i = 0; i < TOTAL_CHARACTERS; i++) {
        if (characterData[i].type == characterType) {
            return characterData[i].spritePath;
        }
    }
    return DEFAULT_SPRITE_PATH;  
}
