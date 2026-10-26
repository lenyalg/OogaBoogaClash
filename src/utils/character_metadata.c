#include "character_metadata.h"

// Mettre chaque attaques avant de les assigner aux personnages
// Definition des segments de frames pour chaque attaque
// Création des attaques avec ces segments

//Format : {startRow, startCol, endRow, endCol} (basé sur index 0)

//Caveman
static const FrameSegment caveman_attack[] = {{4,1,4,4}};
//Humanoid
static const FrameSegment humanoid_attack[] = {{3,4,3,5}, {4,1,4,2}};

static const Attack cavermanAttacks[] = {
  {
    .attackIndex = 0,
    .name = "caveman_slash", 
    .damage = 10.0f, 
    .startupFrames = 4, 
    .activeFrames = 2, 
    .recoveryFrames = 12, 
    .hitboxWidth = 30.0f, 
    .hitboxHeight = 20.0f, 
    .hitboxOffsetX = 15.0f, 
    .hitboxOffsetY = 25.0f, 
    .horizontalKnockback = 600,
    .verticalKnockback = 200,
    .timeStun = 8, // en frames
    .spriteSegments = caveman_attack,
    .segCount = 1,
    .spriteData = {
        {0, 2},  // Sprite 0, 3 frames
        {1, 2},  // Sprite 1, 5 frames
        {2, 14}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  },
  {
    .attackIndex = 1,
    .name = "caveman_smash", 
    .damage = 15.0f, 
    .startupFrames = 7, 
    .activeFrames = 4, 
    .recoveryFrames = 12, 
    .hitboxWidth = 35.0f, 
    .hitboxHeight = 25.0f, 
    .hitboxOffsetX = 45.0f, 
    .hitboxOffsetY = 15.0f,
    .horizontalKnockback = 300,
    .verticalKnockback = 100,
    .timeStun = 8, 
    .spriteSegments = caveman_attack,
    .segCount = 1,
    .spriteData = {
      {0, 3},  // Sprite 0, 3 frames
      {1, 5},  // Sprite 1, 5 frames
      {2, 4}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  }
};

static const Attack girlAttacks[] = {
  {
    .attackIndex = 0,
    .name = "girl_kick",
    .damage = 8.0f,
    .startupFrames = 4, 
    .activeFrames = 2, 
    .recoveryFrames = 12, 
    .hitboxWidth = 25.0f, 
    .hitboxHeight = 15.0f, 
    .hitboxOffsetX = 35.0f, 
    .hitboxOffsetY = 10.0f, 
    .horizontalKnockback = 600,
    .verticalKnockback = 200,
    .timeStun = 8,
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 2},  // Sprite 1, 5 frames
      {2, 14}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  },
  {
    .attackIndex = 1,
    .name = "girl_punch", 
    .damage = 6.0f, 
    .startupFrames = 3, 
    .activeFrames = 2, 
    .recoveryFrames = 6, 
    .hitboxWidth = 20.0f, 
    .hitboxHeight = 10.0f, 
    .hitboxOffsetX = 30.0f, 
    .hitboxOffsetY = 5.0f, 
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 4},  // Sprite 1, 5 frames
      {2, 4}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  }
};

static const Attack eggShellAttacks[] = {
  {
    .attackIndex = 0,
    .name = "egg_shell_peck", 
    .damage = 7.0f, 
    .startupFrames = 4, 
    .activeFrames = 2, 
    .recoveryFrames = 12, 
    .hitboxWidth = 22.0f, 
    .hitboxHeight = 12.0f, 
    .hitboxOffsetX = 32.0f, 
    .hitboxOffsetY = 8.0f, 
    .horizontalKnockback = 600,
    .verticalKnockback = 200,
    .timeStun = 8,
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 2},  // Sprite 1, 5 frames
      {2, 14}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  },
  {
    .attackIndex = 1,
    .name = "egg_shell_roll", 
    .damage = 9.0f, 
    .startupFrames = 6, 
    .activeFrames = 5, 
    .recoveryFrames = 11, 
    .hitboxWidth = 28.0f, 
    .hitboxHeight = 18.0f, 
    .hitboxOffsetX = 38.0f, 
    .hitboxOffsetY = 12.0f, 
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 4},  // Sprite 1, 5 frames
      {2, 4}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  }
};

static const Attack lionAttacks[] = {
  {
    .attackIndex = 0,
    .name = "lion_claw", 
    .damage = 12.0f, 
    .startupFrames = 4, 
    .activeFrames = 2, 
    .recoveryFrames = 12,  
    .hitboxWidth = 35.0f, 
    .hitboxHeight = 25.0f, 
    .hitboxOffsetX = 45.0f, 
    .hitboxOffsetY = 15.0f, 
    .horizontalKnockback = 600,
    .verticalKnockback = 200,
    .timeStun = 8,
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 2},  // Sprite 1, 5 frames
      {2, 14}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  },
  {
    .attackIndex = 1,
    .name = "lion_bite", 
    .damage = 15.0f, 
    .startupFrames = 7, 
    .activeFrames = 5, 
    .recoveryFrames = 14, 
    .hitboxWidth = 40.0f, 
    .hitboxHeight = 30.0f, 
    .hitboxOffsetX = 50.0f, 
    .hitboxOffsetY = 20.0f, 
    .spriteSegments = humanoid_attack,
    .segCount = 2,
    .spriteData = {
      {0, 2},  // Sprite 0, 3 frames
      {1, 4},  // Sprite 1, 5 frames
      {2, 4}   // Sprite 2, 4 frames
    },
    .spriteCount = 3
  }
};

static Character characterDatabase[CHARACTER_COUNT] = {
    // CHARACTER_CAVERMAN
    {
        .characterType = CHARACTER_CAVERMAN,
        .health = 100,
        .speed = 7.0f,
        .jumpImpulse = 750.0f,
        .height = 55.0f,
        .width = 50.0f,
        .weight = 70.0f,
        .attacks = cavermanAttacks,
        .attackCount = 2
    },
    // CHARACTER_CAVERMAN2
    {
        .characterType = CHARACTER_CAVERMAN2,
        .health = 100,
        .speed = 7.0f,
        .jumpImpulse = 750.0f,
        .height = 55.0f,
        .width = 50.0f,
        .weight = 70.0f,
        .attacks = cavermanAttacks,
        .attackCount = 2
    },
    // CHARACTER_GIRL
    {
        .characterType = CHARACTER_GIRL,
        .health = 90,
        .speed = 7.0f,
        .jumpImpulse = 750.0f,
        .height = 50.0f,
        .width = 44.0f,
        .weight = 60.0f,
        .attacks = girlAttacks,
        .attackCount = 2
    },
    // CHARACTER_GIRL2
    {
        .characterType = CHARACTER_GIRL2,
        .health = 90,
        .speed = 7.0f,
        .jumpImpulse = 750.0f,
        .height = 50.0f,
        .width = 44.0f,
        .weight = 60.0f,
        .attacks = girlAttacks,
        .attackCount = 2
    },
    // CHARACTER_EGG_SHELL
    {
        .characterType = CHARACTER_EGG_SHELL,
        .health = 80,
        .speed = 7.0f,
        .jumpImpulse = 780.0f,
        .height = 45.0f,
        .width = 40.0f,
        .weight = 60.0f,
        .attacks = eggShellAttacks,
        .attackCount = 2
    },
    // CHARACTER_EGG_SHELL_2
    {
        .characterType = CHARACTER_EGG_SHELL_2,
        .health = 80,
        .speed = 7.0f,
        .jumpImpulse = 780.0f,
        .height = 45.0f,
        .width = 40.0f,
        .weight = 60.0f,
        .attacks = eggShellAttacks,
        .attackCount = 2
    },
    // CHARACTER_LION
    {
        .characterType = CHARACTER_LION,
        .health = 110,
        .speed = 7.0f,
        .jumpImpulse = 710.0f,
        .height = 60.0f,
        .width = 55.0f,
        .weight = 70.0f,
        .attacks = lionAttacks,
        .attackCount = 2
    },
    // CHARACTER_LION2
    {
        .characterType = CHARACTER_LION2,
        .health = 110,
        .speed = 7.0f,
        .jumpImpulse = 710.0f,
        .height = 60.0f,
        .width = 55.0f,
        .weight = 70.0f,
        .attacks = lionAttacks,
        .attackCount = 2
    }
};

Character GetCharacterMetadata(int characterType) {
    if (characterType < 0 || characterType >= CHARACTER_COUNT) {
        // Retourner un personnage par défaut si le type est invalide
        return characterDatabase[0];
    }
    return characterDatabase[characterType];
}