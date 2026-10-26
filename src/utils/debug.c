#include "debug.h"  



void InitDebug(DebugInfo* debug) {
    debug->enabled = false;
    debug->showPlayerInfo = true;
    debug->showCollisionBoxes = true;
    debug->showSpriteBoxes = true;
    debug->showFPS = true;
    debug->frameByFrame = false;
}

void UpdateDebugInput(DebugInfo* debug) {
    if (IsKeyPressed(KEY_G)) {
      debug->enabled = !debug->enabled;
    }
    
    // Touches supplémentaires pour contrôler ce qui s'affiche
    if (debug->enabled) {
        if (IsKeyPressed(KEY_F1)) debug->showPlayerInfo = !debug->showPlayerInfo;
        if (IsKeyPressed(KEY_F2)) debug->showCollisionBoxes = !debug->showCollisionBoxes;
        if (IsKeyPressed(KEY_F3)) debug->showSpriteBoxes = !debug->showSpriteBoxes;
        if (IsKeyPressed(KEY_F4)) debug->showFPS = !debug->showFPS;
        if (IsKeyPressed(KEY_F5)) debug->frameByFrame = !debug->frameByFrame;
    }
}

// Indique si la simulation doit avancer cette frame
// - Si pas de frame-by-frame: toujours true
// - Si frame-by-frame actif: true seulement quand on appuie sur T
bool DebugShouldStep(DebugInfo* debug) {
    if (!debug->enabled || !debug->frameByFrame) return true;
    return IsKeyPressed(KEY_T);
}

void DrawPlayerDebugInfo(Character* player, float groundY, int playerNum, int yOffset, float platformY) {
  int fontSize = 20;
  int lineH = 30;
  int x = (playerNum == 1) ? 10 : (WINDOWWIDTH - 280); // player 2 drawn à droite pour éviter chevauchement

  DrawText(TextFormat("P%d Y: %.2f", playerNum, player->y), x, yOffset, fontSize, BLACK);
  DrawText(TextFormat("P%d State: %d", playerNum, player->state), x, yOffset + lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d OnGround: %s", playerNum, player->isOnGround ? "YES" : "NO"), x, yOffset + 2 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d VelocityY: %.2f", playerNum, player->velocityY), x, yOffset + 3 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d VelocityX: %.2f", playerNum, player->velocityX), x, yOffset + 4 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d Health: %.2f", playerNum, player->health), x, yOffset + 5 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d x: %.2f", playerNum, player->x), x, yOffset + 6 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d y: %.2f", playerNum, player->y), x, yOffset + 7 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d bottom y: %.2f", playerNum, player->y + player->height), x, yOffset + 8 * lineH, fontSize, BLACK);
  DrawText(TextFormat("Ground Y: %.2f", groundY), x, yOffset + 9 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d DropThru: %s", playerNum, player->dropThroughPlatform ? "YES" : "NO"), x, yOffset + 10 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d PlatformID: %d", playerNum, player->standingPlatformId), x, yOffset + 11 * lineH, fontSize, BLACK);
  DrawText(TextFormat("Platform Y: %.2f", platformY), x, yOffset + 12 * lineH, fontSize, BLACK);
  DrawText(TextFormat("P%d DoubleJump: %s", playerNum, player->hasDoubleJumped ? "YES" : "NO"), x, yOffset + 13 * lineH, fontSize, BLACK);
}

void DrawPlayerCollisionBoxes(Character* player, Color outlineColor, Color spriteBoxColor, DebugInfo* debug) {
    DrawRectangleLines(player->x, player->y, player->width, player->height, outlineColor);
    
    if (player->spriteMetadata && debug->showSpriteBoxes) {
        float spriteBoxX = player->spriteX - (player->spriteMetadata->frameWidth - player->width) / 2;
        float spriteBoxY = player->spriteY - (player->spriteMetadata->frameHeight - player->height) + player->spriteMetadata->anchorOffsetY;
        
        DrawRectangle(
            spriteBoxX, 
            spriteBoxY, 
            player->spriteMetadata->frameWidth, 
            player->spriteMetadata->frameHeight, 
            spriteBoxColor
        );
    }
}

void DrawDebugFPS(void) {
    DrawText(TextFormat("FPS: %d", GetFPS()), WINDOWWIDTH - 80, 20, 20, LIME);
}


void DrawDebugOverlay(DebugInfo* debug, Character* player1, Character* player2, float groundY, float platformY) {
    if (!debug->enabled) return;
    
    // Informations des joueurs
    if (debug->showPlayerInfo) {
        DrawPlayerDebugInfo(player1, groundY, 1, 10, platformY);
        if (player2 != NULL) {
            DrawPlayerDebugInfo(player2, groundY, 2, 100, platformY);
        }
    }
    
    // Boîtes de collision
    if (debug->showCollisionBoxes) {
        DrawPlayerCollisionBoxes(player1, BLUE, (Color){255, 0, 0, 50}, debug);
        if (player2 != NULL) {
            DrawPlayerCollisionBoxes(player2, GREEN, (Color){0, 255, 0, 50}, debug);
        }
    }
    
    // FPS
    if (debug->showFPS) {
        DrawDebugFPS();
    }
    
    // Aide
    DrawText("Debug: G=Toggle | F1=Info | F2=Collision | F3=Sprite | F4=FPS", 
             WINDOWWIDTH - 460, 40, 16, DARKGRAY);
    if (debug->frameByFrame) {
        DrawText("Frame-by-frame: T pour avancer", WINDOWWIDTH - 320, 60, 16, RED);
    }
}

void FrameByFrameControl(DebugInfo* debug) {
    if (!debug->enabled || !debug->frameByFrame) return;
    
    static bool stepRequested = false;
    
    if (IsKeyPressed(KEY_F6)) {
        stepRequested = true;
    }
    
    if (!stepRequested) {
        while (!WindowShouldClose()) {
            // Attendre la prochaine frame
            if (IsKeyPressed(KEY_F6)) {
                stepRequested = true;
                break;
            }
        }
    }
    
    stepRequested = false;
}

