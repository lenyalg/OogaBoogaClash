#include "options.h"
#include <stddef.h>

void InitOptionsState(OptionsState* options) {
    options->player1InputType = INPUT_KEYBOARD;
    options->player2InputType = INPUT_KEYBOARD;
    options->player1GamepadId = 0;
    options->player2GamepadId = 1;
    options->selectedOption = 0;
}

void UpdateOptionsMenu(OptionsState* options) {
    // Navigation
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        options->selectedOption = (options->selectedOption + 1) % 2;
    }
    
    // Changer le type d'input
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        if (options->selectedOption == 0) {
            options->player1InputType = (options->player1InputType == INPUT_KEYBOARD) ? INPUT_GAMEPAD : INPUT_KEYBOARD;
        } else {
            options->player2InputType = (options->player2InputType == INPUT_KEYBOARD) ? INPUT_GAMEPAD : INPUT_KEYBOARD;
        }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        if (options->selectedOption == 0) {
            options->player1InputType = (options->player1InputType == INPUT_KEYBOARD) ? INPUT_GAMEPAD : INPUT_KEYBOARD;
        } else {
            options->player2InputType = (options->player2InputType == INPUT_KEYBOARD) ? INPUT_GAMEPAD : INPUT_KEYBOARD;
        }
    }
    
    // Changer l'ID de la manette (si gamepad sélectionné)
    if (IsKeyPressed(KEY_ENTER)) {
        if (options->selectedOption == 0 && options->player1InputType == INPUT_GAMEPAD) {
            int newGamepadId = (options->player1GamepadId + 1) % 4;
            options->player1GamepadId = (newGamepadId == options->player2GamepadId) ? (newGamepadId + 1) % 4 : newGamepadId;
        } else if (options->selectedOption == 1 && options->player2InputType == INPUT_GAMEPAD) {
            int newGamepadId = (options->player2GamepadId + 1) % 4;
            options->player2GamepadId = (newGamepadId == options->player1GamepadId) ? (newGamepadId + 1) % 4 : newGamepadId;
        }
    }
}

static void DrawKeyBox(const char *text, int x, int y) {
    int width = MeasureText(text, 25) + 20;
    DrawRectangleRounded((Rectangle){x, y, width, 40}, 0.3f, 10, LIGHTGRAY);
    DrawRectangleRoundedLines((Rectangle){x, y, width, 40}, 0.3f, 10, DARKGRAY);
    DrawText(text, x + 10, y + 7, 25, BLACK);
}

void DrawOptionsMenu(Texture2D background, OptionsState* options){
    // Dessiner le background passé en paramètre
    DrawTexture(background, 0, 0, WHITE);

    // Rectangle semi-transparent
    DrawRectangleRounded((Rectangle){1280*0.1, 720*0.45, 1280*0.8, 720*0.45}, 0.10f, 20, Fade(RAYWHITE, 0.1f));
    DrawRectangleRoundedLines((Rectangle){1280*0.1, 720*0.45, 1280*0.8, 720*0.45}, 0.10f, 20, Fade(GRAY, 0.6f));

    DrawText("OPTIONS", 500, 80, 60, BLACK);
    
    // Section choix d'input en HAUT - Fond pour la lisibilité
    DrawRectangleRounded((Rectangle){1280*0.1, 170, 700, 145}, 0.15f, 10, Fade(RAYWHITE, 0.1f));
    DrawRectangleRoundedLines((Rectangle){1280*0.1, 170, 700, 145}, 0.15f, 10, Fade(GRAY, 0.6f));
    
    DrawText("CHOIX DES PERIPHERIQUES :", 1280*0.1 + 50, 180, 30, BLACK);
    
    // Instructions générales
    DrawText("< Fleches/ZQSD : Changer | Entree : Changer ID manette >", 1280*0.1 + 50, 215, 18, BLACK);
    
    // Joueur 1
    Color p1Color = (options->selectedOption == 0) ? RED : BLACK;
    const char* p1Prefix = (options->selectedOption == 0) ? "> Joueur 1 : " : "  Joueur 1 : ";
    DrawText(p1Prefix, 1280*0.1 + 50, 250, 25, p1Color);
    
    const char* p1InputText;
    if (options->player1InputType == INPUT_KEYBOARD) {
        p1InputText = "CLAVIER";
    } else {
        const char* gamepadName = GetGamepadName(options->player1GamepadId);
        if (gamepadName != NULL && gamepadName[0] != '\0') {
            p1InputText = TextFormat("%s (ID %d)", gamepadName, options->player1GamepadId);
        } else {
            p1InputText = TextFormat("MANETTE %d (Deconnectee)", options->player1GamepadId);
        }
    }
    DrawText(p1InputText, 1280*0.1 + 240, 250, 22, p1Color);
    
    // Joueur 2
    Color p2Color = (options->selectedOption == 1) ? RED : BLACK;
    const char* p2Prefix = (options->selectedOption == 1) ? "> Joueur 2 : " : "  Joueur 2 : ";
    DrawText(p2Prefix, 1280*0.1 + 50, 290, 25, p2Color);
    
    const char* p2InputText;
    if (options->player2InputType == INPUT_KEYBOARD) {
        p2InputText = "CLAVIER";
    } else {
        const char* gamepadName = GetGamepadName(options->player2GamepadId);
        if (gamepadName != NULL && gamepadName[0] != '\0') {
            p2InputText = TextFormat("%s (ID %d)", gamepadName, options->player2GamepadId);
        } else {
            p2InputText = TextFormat("MANETTE %d (Deconnectee)", options->player2GamepadId);
        }
    }
    DrawText(p2InputText, 1280*0.1 + 240, 290, 22, p2Color);
    
    DrawText("CONTROLES DU JEU :", 1280*0.1 + 50, 750*0.45 + 20, 35, BLACK);

    // JOUEUR 1
    DrawText("Joueur 1 :", 1280*0.1 + 50, 750*0.45 + 80, 30, BLACK);
    // Déplacement (Haut, Bas, Gauche, Droite)
    DrawText("Deplacement :", 1280*0.1 + 50, 750*0.45 + 130, 25, BLACK);
    if (options->player1InputType == INPUT_KEYBOARD) {
        DrawKeyBox("Z", 1280*0.1 + 250, 750*0.45 + 125); // Haut
        DrawKeyBox("S", 1280*0.1 + 310, 750*0.45 + 125); // Bas
        DrawKeyBox("Q", 1280*0.1 + 370, 750*0.45 + 125); // Gauche
        DrawKeyBox("D", 1280*0.1 + 430, 750*0.45 + 125); // Droite
    } else {
        DrawText("Stick Gauche", 1280*0.1 + 250, 750*0.45 + 130, 25, BLACK);
    }
    // Saut
    DrawText("Saut :", 1280*0.1 + 50, 750*0.45 + 180, 25, BLACK);
    if (options->player1InputType == INPUT_KEYBOARD) {
        DrawKeyBox("Z", 1280*0.1 + 250, 750*0.45 + 175);
    } else {
        DrawText("Bouton A", 1280*0.1 + 250, 750*0.45 + 180, 25, BLACK);
    }
    // Attaque
    DrawText("Attaque :", 1280*0.1 + 50, 750*0.45 + 230, 25, BLACK);
    if (options->player1InputType == INPUT_KEYBOARD) {
        DrawKeyBox("E", 1280*0.1 + 250, 750*0.45 + 225);
    } else {
        DrawText("Bouton X", 1280*0.1 + 250, 750*0.45 + 230, 25, BLACK);
    }


    // JOUEUR 2
    DrawText("Joueur 2 :", 1280*0.5 + 50, 750*0.45 + 80, 30, BLACK);
    // Déplacement (Haut, Bas, Gauche, Droite)
    DrawText("Deplacement :", 1280*0.5 + 50, 750*0.45 + 130, 25, BLACK);
    if (options->player2InputType == INPUT_KEYBOARD) {
        DrawKeyBox("O", 1280*0.5 + 250, 750*0.45 + 125);   // Haut
        DrawKeyBox("L", 1280*0.5 + 310, 750*0.45 + 125);   // Bas
        DrawKeyBox("K", 1280*0.5 + 370, 750*0.45 + 125);   // Gauche
        DrawKeyBox("M", 1280*0.5 + 430, 750*0.45 + 125);   // Droite
    } else {
        DrawText("Stick Gauche", 1280*0.5 + 250, 750*0.45 + 130, 25, BLACK);
    }
    // Saut
    DrawText("Saut :", 1280*0.5 + 50, 750*0.45 + 180, 25, BLACK);
    if (options->player2InputType == INPUT_KEYBOARD) {
        DrawKeyBox("O", 1280*0.5 + 250, 750*0.45 + 175);
    } else {
        DrawText("Bouton A", 1280*0.5 + 250, 750*0.45 + 180, 25, BLACK);
    }
    // Attaque
    DrawText("Attaque :", 1280*0.5 + 50, 750*0.45 + 230, 25, BLACK);
    if (options->player2InputType == INPUT_KEYBOARD) {
        DrawKeyBox("I", 1280*0.5 + 250, 750*0.45 + 225);
    } else {
        DrawText("Bouton X", 1280*0.5 + 250, 750*0.45 + 230, 25, BLACK);
    }

    DrawText("Appuie sur ESPACE pour revenir au menu", 1280*0.1 + 200, 750*0.45 + 280, 25, BLACK);
}
