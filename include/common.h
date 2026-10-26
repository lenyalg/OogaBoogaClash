#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include "sprite_manager.h"

#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720
#define ANIMATION_SPEED 8
// Vitesse d'animation spécifique pour le dash (frames par seconde)
#define ANIMATION_SPEED_DASH 6

// États possibles pour l'animation
typedef enum {
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_JUMP,
    ANIM_FALL,
    ANIM_ATTACK,
    ANIM_DASH,
    ANIM_HIT
} AnimState;

typedef struct {
  int spriteIndex;
  int duration;
} ActionSpriteData;

typedef struct {
  int attackIndex;
  const char* name;
  float damage;
  int startupFrames;
  int activeFrames;
  int recoveryFrames;
  float hitboxWidth;
  float hitboxHeight;
  float hitboxOffsetX;
  float hitboxOffsetY;
  float horizontalKnockback;
  float verticalKnockback;
  float timeStun;
  const FrameSegment *spriteSegments;
  int segCount;
  ActionSpriteData spriteData[20];
  int spriteCount;
} Attack;

typedef struct {
  int spriteCount;
  int currentSpriteIndex;;
  int framesInCurrentSprite;
  const Attack *attackData;
} AttackAnimationData;


typedef enum InputAction {
    DOWN,
    JUMP,
    LEFT,
    RIGHT,
    ATTACK
} InputAction;

typedef enum {
    INPUT_KEYBOARD,
    INPUT_GAMEPAD
} InputType;

typedef struct {
    InputType type;
    int gamepadId; 
    bool isConnected;
    bool wasDownPressedLastFrame;
    float downInputTimer;
} PlayerInput;

// --- structures menu ---
typedef enum {
    STATE_MENU,
    STATE_CHARACTER_SELECT,
    STATE_MAP,
    STATE_GAME1V1,
    STATE_OPTIONS,
    STATE_GAMEOVER,
    STATE_EXIT,
    STATE_SURVIVAL
} GameState;

typedef struct {
    Rectangle bounds;
    const char *text;
    bool hovered;
    Texture2D img;
    float scale; 
} Button;


typedef struct {
    //Metadata
    int characterType;
    float width;
    float height;
    float speed;
    float health;
    float jumpImpulse;  
    float weight;
    int stock;
    float maxHealth;

    // Input
    PlayerInput input;

    // Physique
    float x;
    float y;
    float previousY; 
    float velocityY;
    float velocityX;
    int standingPlatformId;
    bool isOnGround;
    float maxFallSpeed;
    bool isFastFalling;
    float yBeforeJump;
    bool dropThroughPlatform;
    bool hasDoubleJumped;
    float fastFallImpulse;
    float moving;
    
    // Animation
    int currentFrame;
    int framesCounter;
    bool isFacingRight;
    bool isP2;
    AnimState state;
    AnimState previousState;
    
    // Combat
    bool isAttacking;
    AttackAnimationData currentAttack;
    bool hasHitThisAttack; 
    const Attack *attacks;
    int attackCount;
    bool isHited;
    float timerHitstun;
    bool isDashing;
    int dashFramesRemaining;
    int dashDurationFrames;
    int dashCooldownFramesRemaining;
    int dashCooldownFrames;
    int dashDirection;
    int dashSpeed;
    
    // Sprite management
    float spriteX;
    float spriteY;
    SpriteMetadata *spriteMetadata;
} Character;

#endif