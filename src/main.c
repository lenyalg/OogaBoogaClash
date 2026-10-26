#include "raylib.h"
#include "common.h"
#include "physics.h"
#include "animation.h"
#include "collision.h"
#include "options.h"
#include "player.h"
#include "menudebut.h"
#include "sprite_manager.h"
#include "debug.h"
#include "character_select.h"
#include "attack.h"
#include "map_choice.h"
#include "map_metadata.h"
#include "input.h"
#include "hud_system.h"
#include "menugameover.h"
#include "sauvegarde.h"
#include "sound_manager.h"
#include "fx_manager.h"
#include <stdlib.h>
#include "survie.h"
#include "timesurvie.h"


int main()
{

  // --- INITIALISATION DE LA FENETRE ---
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(WINDOWWIDTH, WINDOWHEIGHT , "OOGA BOOGA CLASH");
  SetTargetFPS(60);

  // --- INITIALISATION DE L'AUDIO ---
  InitAudioDevice();
  InitSoundManager();

  // --- CHARGEMENT DE LA SAUVEGARDE ---
  SauvegardeData sauvegarde;
  LoadSauvegarde(&sauvegarde);

  // --- INITIALISATION DU MENU ---
  InitMenu();

  // --- CHARGEMENT DES TEXTURES ---
  Texture2D menuBackground = LoadTexture("assets/menu/Menu_Background.png");
  Texture2D menuBackground2 = LoadTexture("assets/menu/Wood_Background.png");

  // --- INITIALISATION DES VARIABLES DE JEU ---
  Character player1;
  Character player2;
  Texture2D spritePlayer1;
  Texture2D spritePlayer2;
  Texture2D map;
  spritePlayer1.id = 0;
  spritePlayer2.id = 0;
  map.id = 0;

  MapMetadata* currentMap = NULL;
  FXManager fxManager;

  float gravity = 1650.0f;

  DebugInfo debugInfo;
  InitDebug(&debugInfo);

  GameState state = STATE_MENU;

  CharacterSelectState charSelectState;
  InitCharacterSelect(&charSelectState);
  
  OptionsState optionsState;
  InitOptionsState(&optionsState);
  

  MapSelectState mapState;
  InitMapSelect(&mapState);

  HealthBar player1HealthBar;
  HealthBar player2HealthBar;
   
  GameOverState gameOver;

  Texture2D player1stockTexture;
  Texture2D player2stockTexture;

  Timesurvival survivalTime;

  InventoryBar player1InventoryBar = HUD_InitInventoryBar(50, 120, 24, 24, 1);
  InventoryBar player2InventoryBar = HUD_InitInventoryBar(1036, 120, 24, 24, 1);

  MeteoriteData* meteorites = (MeteoriteData*)malloc(sizeof(MeteoriteData));
  bool hasSurvivalInit = false;
  
  while (!WindowShouldClose())        
  {
    float dt = GetFrameTime();
    UpdateSoundManager();

    
    // --- ETATS DU JEU ---
    switch(state)
    {
      case STATE_MENU:
        UpdateMenu(&state);
        break;

      case STATE_OPTIONS:
        UpdateOptionsMenu(&optionsState);
        if (IsKeyPressed(KEY_SPACE)) state = STATE_MENU;
        break;

      case STATE_CHARACTER_SELECT:
        UpdateCharacterSelect(&charSelectState);

        if (IsKeyPressed(KEY_ENTER)) {
          InitGameOver(&gameOver);
          currentMap = GetMapMetadata(MAP_CAVE);   // choisi une map qui existe par défaut
          map = LoadTexture(GetMapPath(MAP_CAVE));
          state = STATE_MAP;
        }
        if (IsKeyPressed(KEY_SPACE)) state = STATE_MENU;
          break;

      case STATE_MAP:
        UpdateMapSelect(&mapState);
        int selected = GetSelectedMap(&mapState);

        if (selected != -1) {
            currentMap = GetMapMetadata((MapType)selected);

            if (map.id != 0) {
              UnloadTexture(map);
            }

            map = LoadTexture(GetMapPath(selected));
            InitPlayer(&player1, currentMap->playerSpawns[0].x, currentMap->playerSpawns[0].y, charSelectState.player1Selected, false);
            InitPlayer(&player2, currentMap->playerSpawns[1].x, currentMap->playerSpawns[1].y, charSelectState.player2Selected, true);
          
            // initialiser les inputs selon les options choisies
            InitPlayerInput(&player1.input, optionsState.player1InputType, optionsState.player1GamepadId);
            InitPlayerInput(&player2.input, optionsState.player2InputType, optionsState.player2GamepadId);
          
            spritePlayer1 = LoadTexture(GetSpritePath(player1.characterType));
            spritePlayer2 = LoadTexture(GetSpritePath(player2.characterType));

            player1HealthBar = HUD_InitHealthBar(&player1.health, 50, 50, 300, 10, 10, 10, 1);
            player2HealthBar = HUD_InitHealthBar(&player2.health, 930, 50, 300, 10, 10, 10, -1);
            
            StartBackgroundMusic();
            InitFXManager(&fxManager);
            
            state = STATE_GAME1V1;

            const char *p1_portrait = GetPortraitPath(player1.characterType);
            const char *p2_portrait = GetPortraitPath(player2.characterType);
            
            player1stockTexture = LoadTexture(p1_portrait);
            player2stockTexture = LoadTexture(p2_portrait);
        }

        if (IsKeyPressed(KEY_SPACE)) {
            state = STATE_MENU;
        }
      break;

      case STATE_GAME1V1:

        sauvegarde.tempsjeu += dt;
        
        if (IsKeyPressed(KEY_SPACE)) {
          // nettoyage du mode 1v1
          if (spritePlayer1.id != 0) {
            UnloadTexture(spritePlayer1);
            spritePlayer1.id = 0;
          }
          if (spritePlayer2.id != 0) {
            UnloadTexture(spritePlayer2);
            spritePlayer2.id = 0;
          }
          if (map.id != 0) {
            UnloadTexture(map);
            map.id = 0;
          }
          if (player1stockTexture.id != 0) {
            UnloadTexture(player1stockTexture);
            player1stockTexture.id = 0;
          }
          if (player2stockTexture.id != 0) {
            UnloadTexture(player2stockTexture);
            player2stockTexture.id = 0;
          }

          HUD_UnloadHealthBar(&player1HealthBar);
          HUD_UnloadHealthBar(&player2HealthBar);
          currentMap = NULL;
          state = STATE_MENU;
        }

        UpdateDebugInput(&debugInfo);
        bool allowSim = DebugShouldStep(&debugInfo);
        
        if (allowSim && currentMap != NULL) {
          UpdateGamepadConnections(&player1.input, &player2.input);

          MovePlayer(&player1, gravity, &fxManager);
          MovePlayer(&player2, gravity, &fxManager);
      
          UpdatePhysics(&player1, dt, gravity, state);
          UpdatePhysics(&player2, dt, gravity, state);

          for (int i = 1; i < currentMap->platformCount; i++)
          {
            CheckCollisionPlateform(&player1, i, currentMap->platforms[i].y, currentMap->platforms[i].x, currentMap->platforms[i].width, currentMap->platforms[0].y);
            CheckCollisionPlateform(&player2, i, currentMap->platforms[i].y, currentMap->platforms[i].x, currentMap->platforms[i].width, currentMap->platforms[0].y);
          }
          
          CheckCollisionsGround(&player1, currentMap->platforms[0].y, currentMap->platforms[0].x, currentMap->platforms[0].width);
          CheckCollisionsGround(&player2, currentMap->platforms[0].y, currentMap->platforms[0].x, currentMap->platforms[0].width);
      
          CheckVoid(&player1, currentMap);
          CheckVoid(&player2, currentMap);

          attackClassic(&player1);
          attackClassic(&player2);
          
          UpdateAttack(&player1);
          UpdateAttack(&player2);
          
          if (CheckAttackCollision(&player1, &player2)) {
            DealDamage(&player1, &player2, currentMap, &fxManager);
          }
          if (CheckAttackCollision(&player2, &player1)) {
            DealDamage(&player2, &player1, currentMap, &fxManager);
          }

          if (player1.isHited)
          {
              player1.timerHitstun -= dt * 60.0f;
              if (player1.timerHitstun <= 0) {
                  player1.isHited = false;
                  player1.timerHitstun = 0.0f;
              }
          }
          if (player2.isHited)
          {
              player2.timerHitstun -= dt * 60.0f;
              if (player2.timerHitstun <= 0) {
                  player2.isHited = false;
                  player2.timerHitstun = 0.0f;
              }
          }
          if (player1.velocityX > 0) {
            player1.velocityX -= 5.0f;
          } else if (player1.velocityX < 0) {
            player1.velocityX += 5.0f;
          }

          if (player2.velocityX > 0) {
            player2.velocityX -= 5.0f;
          } else if (player2.velocityX < 0) {
            player2.velocityX += 5.0f;
          }

          if (player1.isOnGround && !player1.isHited) {
              player1.velocityX = 0.0f;
          }
          if (player2.isOnGround && !player2.isHited) {
              player2.velocityX = 0.0f;
          }
          
          UpdateAnimState(&player1, player1.moving);
          UpdateAnimState(&player2, player2.moving);
          UpdateFXManager(&fxManager);

        }
        if (player1.stock <= 0)
        {
          sauvegarde.player2Win += 1;
          sauvegarde.nbtotparties += 1;
          SauvegardeGame(&sauvegarde);

          StopBackgroundMusic();
          
          gameOver.winnerText = "JOUEUR J2 GAGNE !";
          gameOver.spritewinner = player2stockTexture;
          gameOver.active = true;
          
          // Nettoyage des ressources du mode 1v1
          if (spritePlayer1.id != 0) {
            UnloadTexture(spritePlayer1);
            spritePlayer1.id = 0;
          }
          if (spritePlayer2.id != 0) {
            UnloadTexture(spritePlayer2);
            spritePlayer2.id = 0;
          }
          if (map.id != 0) {
            UnloadTexture(map);
            map.id = 0;
          }
          HUD_UnloadHealthBar(&player1HealthBar);
          HUD_UnloadHealthBar(&player2HealthBar);
          currentMap = NULL;
          
          state = STATE_GAMEOVER;
        }
        else if (player2.stock <= 0)
        {
          sauvegarde.player1Win += 1;
          sauvegarde.nbtotparties += 1;
          SauvegardeGame(&sauvegarde);

          StopBackgroundMusic();
          
          gameOver.winnerText = "JOUEUR J1 GAGNE !";
          gameOver.spritewinner = player1stockTexture;
          gameOver.active = true;
          
          // Nettoyage des ressources du mode 1v1
          if (spritePlayer1.id != 0) {
            UnloadTexture(spritePlayer1);
            spritePlayer1.id = 0;
          }
          if (spritePlayer2.id != 0) {
            UnloadTexture(spritePlayer2);
            spritePlayer2.id = 0;
          }
          if (map.id != 0) {
            UnloadTexture(map);
            map.id = 0;
          }
          HUD_UnloadHealthBar(&player1HealthBar);
          HUD_UnloadHealthBar(&player2HealthBar);
          currentMap = NULL;
          
          state = STATE_GAMEOVER;
        }
        break;

      case STATE_SURVIVAL:
        if (!hasSurvivalInit) {
          Survival_Init(&survivalTime);
          currentMap = GetMapMetadata(MAP_SURVIVAL);
          map = LoadTexture("assets/Backgrounds/survie.png");
          InitMeteoriteData(meteorites, 100, 2.0f);  
          InitGameOver(&gameOver);
          InitPlayer(&player1, currentMap->playerSpawns[0].x, currentMap->playerSpawns[0].y, 0, false);
          InitPlayerInput(&player1.input, optionsState.player1InputType, optionsState.player1GamepadId);
          spritePlayer1 = LoadTexture(GetSpritePath(player1.characterType));
          player1HealthBar = HUD_InitHealthBar(&player1.health, 50, 50, 300, 10, 10, 10, 1);
          hasSurvivalInit = true;
        }
        
        if (IsKeyPressed(KEY_SPACE)) {
          // Nettoyage du mode survival
          if (spritePlayer1.id != 0) {
            UnloadTexture(spritePlayer1);
            spritePlayer1.id = 0;
          }
          if (map.id != 0) {
            UnloadTexture(map);
            map.id = 0;
          }
          HUD_UnloadHealthBar(&player1HealthBar);
          if (meteorites->meteoriteActive != NULL) {
            free(meteorites->meteoriteActive);
            meteorites->meteoriteActive = NULL;
          }
          hasSurvivalInit = false;
          currentMap = NULL;
          state = STATE_MENU;
          break;
        }
        
        UpdateDebugInput(&debugInfo);
        bool allowSimSurvival = DebugShouldStep(&debugInfo);
        
        if (allowSimSurvival && currentMap != NULL) {
          UpdateGamepadConnections(&player1.input, &player2.input);

          Survival_Update(&survivalTime, dt);

          MovePlayer(&player1, gravity, &fxManager);
          
          UpdatePhysics(&player1, dt, gravity, state);
          
          CheckCollisionsGround(&player1, currentMap->platforms[0].y, currentMap->platforms[0].x, currentMap->platforms[0].width);
          
          UpdateAnimState(&player1, player1.moving);
          CheckMeteoriteCollisions(&player1, meteorites);
          meteoriteHandler(meteorites, dt);
        
          meteorites->meteoriteTimer += dt;
          if (meteorites->meteoriteTimer >= meteorites->spawnInterval) {
            for (int i = 0; i < 5; i++)
            {
              meteoriteAdd(meteorites);
            }
            meteorites->meteoriteTimer = 0.0f;
            if (meteorites->spawnInterval > 0.3f){
              meteorites->spawnInterval *= 0.95f;
            }
          }
        }        
        
        if (player1.health < 1)
        {
          Survival_Stop(&survivalTime);
          survivalTime.scoreInt = (int)survivalTime.score;
          gameOver.timer = survivalTime.scoreInt;
          if (survivalTime.scoreInt > sauvegarde.meilleurscoresolo) {
            sauvegarde.meilleurscoresolo = survivalTime.scoreInt;
          }
          SauvegardeGame(&sauvegarde);
          // Nettoyage des ressources du mode survival
          if (spritePlayer1.id != 0) {
            UnloadTexture(spritePlayer1);
            spritePlayer1.id = 0;
          }
          if (map.id != 0) {
            UnloadTexture(map);
            map.id = 0;
          }
          HUD_UnloadHealthBar(&player1HealthBar);
          if (meteorites->meteoriteActive != NULL) {
            free(meteorites->meteoriteActive);
            meteorites->meteoriteActive = NULL;
          }
          hasSurvivalInit = false;
          currentMap = NULL;
          gameOver.active = true;
          state = STATE_GAMEOVER;
        }
      break;

      case STATE_GAMEOVER:
        DrawGameOver(&gameOver, WINDOWWIDTH);
        UpdateGameOver(&gameOver, (int*)&state);
      break;


      case STATE_EXIT:
        CloseWindow();
        return 0;
    }

  // --- AFFICHAGE ---
  BeginDrawing();
  ClearBackground(WHITE);

  switch(state)
  {
    case STATE_MENU:
      DrawMenu();
      DrawSauvegarde(&sauvegarde);

      break;

    case STATE_CHARACTER_SELECT:
      DrawCharacterSelect(&charSelectState);
      break;

    case STATE_MAP:
      DrawMapSelect(&mapState);
      break;

    case STATE_GAME1V1:
      if (currentMap != NULL) {
          DrawTexture(map, 0, 0, WHITE);
          for (int i = 0; i < currentMap->platformCount; i++) {
              DrawRectangleRec(currentMap->platforms[i], (Color){0, 0, 0, 0});
          }

          DrawCharacter(&player1, spritePlayer1, WHITE);
          DrawCharacter(&player2, spritePlayer2, WHITE);
          
          DrawFXManager(&fxManager);
          
          HUD_DrawHealthBar(&player1HealthBar);
          HUD_DrawHealthBar(&player2HealthBar);
          HUD_DrawInventoryBar(&player1InventoryBar, player1.dashCooldownFramesRemaining, player1.characterType, spritePlayer1);
          HUD_DrawInventoryBar(&player2InventoryBar, player2.dashCooldownFramesRemaining, player2.characterType, spritePlayer2);
          HUD_DrawStock(player1.stock, 40, 80, player1stockTexture, false);
          HUD_DrawStock(player2.stock, 1220, 80, player2stockTexture, true);

          DrawDebugOverlay(&debugInfo, &player1, &player2, currentMap->platforms[0].y, currentMap->platforms[0].y);
      }
      break;

      case STATE_SURVIVAL:
      if (currentMap != NULL) {
          DrawTexture(map, 0, 0, WHITE);
          DrawRectangleRec(currentMap->platforms[0], (Color){0, 0, 0, 0});
          DrawCharacter(&player1, spritePlayer1, WHITE);

          Survival_DrawHUD(&survivalTime);
          
          HUD_DrawHealthBar(&player1HealthBar);
          HUD_DrawInventoryBar(&player1InventoryBar, player1.dashCooldownFramesRemaining, player1.characterType, spritePlayer1);
          
          meteoriteDraw(meteorites);
          
          if (debugInfo.enabled) {
            DrawDebugOverlay(&debugInfo, &player1, NULL, currentMap->platforms[0].y, currentMap->platforms[0].y);
          }
      }
      break;

    case STATE_GAMEOVER:
        DrawGameOver(&gameOver, WINDOWWIDTH);
        UpdateGameOver(&gameOver, (int*)&state);

      break;

    case STATE_OPTIONS:
      DrawOptionsMenu(menuBackground2, &optionsState);
      break;

    case STATE_EXIT:
      break;
  }

  EndDrawing();
}

  // --- CLEANUP ---
  UnloadSoundManager();
  CloseAudioDevice();
  
  UnloadMenu();
  UnloadMapSelect(&mapState);

  if (spritePlayer1.id != 0) UnloadTexture(spritePlayer1);
  if (spritePlayer2.id != 0) UnloadTexture(spritePlayer2);

  if (map.id != 0) UnloadTexture(map);
  
  if (player1stockTexture.id != 0) UnloadTexture(player1stockTexture);
  if (player2stockTexture.id != 0) UnloadTexture(player2stockTexture);

  // libération du fond d'options
  UnloadTexture(menuBackground);
  UnloadTexture(menuBackground2);
  
  // libération des barres de vie (seulement si elles n'ont pas déjà été libérées)
  HUD_UnloadHealthBar(&player1HealthBar);
  HUD_UnloadHealthBar(&player2HealthBar);

  UnloadGameOver(&gameOver);

  // libération des barres d'inventaire
  HUD_UnloadInventoryBar(&player1InventoryBar);
  HUD_UnloadInventoryBar(&player2InventoryBar);

  // libération des météorites
  if (meteorites != NULL) {
    if (meteorites->meteoriteActive != NULL) {
      free(meteorites->meteoriteActive);
    }
    free(meteorites);
  }

  CloseWindow();
  return 0;
}
