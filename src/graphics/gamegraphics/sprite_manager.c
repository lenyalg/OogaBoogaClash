#include "sprite_manager.h"
#include "common.h" 
#include <stddef.h>

// DÉFINITION DES MÉTADONNÉES POUR CHAQUE PERSONNAGE
// Chaque frameWidth et frameHeight est calculé en divisant la taille totale par le nombre de frames soit 6 et 7

static SpriteMetadata spriteDatabase[CHARACTER_COUNT] = {
    // CHARACTER_CAVERMAN
    {
        .name = "caverman",
        .frameWidth = 97.0f,       
        .frameHeight = 71.0f,          
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 582.0f,
        .spriteHeight = 497.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_CAVERMAN2
    {
        .name = "caverman2",
        .frameWidth = 97.0f,
        .frameHeight = 71.0f,
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 582.0f,
        .spriteHeight = 497.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_GIRL
    {
        .name = "girl",
        .frameWidth = 99.0f,          
        .frameHeight = 74.0f,          
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 594.0f,
        .spriteHeight = 518.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_GIRL2
    {
        .name = "girl2",
        .frameWidth = 99.0f,
        .frameHeight = 74.0f,
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 594.0f,
        .spriteHeight = 518.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_EGG_SHELL
    {
        .name = "egg-shell",
        .frameWidth = 107.0f,        
        .frameHeight = 71.0f,          
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 642.0f,
        .spriteHeight = 497.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_EGG_SHELL_2
    {
        .name = "egg-shell-2",
        .frameWidth = 107.0f,
        .frameHeight = 71.0f,
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 642.0f,
        .spriteHeight = 497.0f,
        .anchorOffsetY = 6.0f
    },
    // CHARACTER_LION
    {
        .name = "lion",
        .frameWidth = 120.0f,         
        .frameHeight = 83.0f,          
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 720.0f,
        .spriteHeight = 581.0f,
        .anchorOffsetY = 11.0f
    },
    // CHARACTER_LION2
    {
        .name = "lion2",
        .frameWidth = 120.0f,
        .frameHeight = 83.0f,
        .framesPerRow = 6,
        .rowCount = 7,
        .spriteWidth = 720.0f,
        .spriteHeight = 581.0f,
        .anchorOffsetY = 11.0f
    }
};


// TABLEAUX DE SEGMENTS D'ANIMATION PAR PERSONNAGE
// Format : {startRow, startCol, endRow, endCol} (basé sur index 0)


// --- TEMPLATE 1: Caveman (6+1 idle, 1+2 run, 3 jump, 2 fall) fonctionne aussi avec la paire 
static const FrameSegment caveman_idle[] = {{0,0,0,5}, {1,0,1,0}};
static const FrameSegment caveman_run[]  = {{1,1,2,1}};
static const FrameSegment caveman_jump[] = {{2,2,2,4}};
static const FrameSegment caveman_fall[] = {{2,5,3,1}};
static const FrameSegment caveman_dash[] = {{3,2,3,5},{4,1,4,1}};
static const FrameSegment caveman_hit[] = {{4,5,4,5},{5,0,5,0}};

// --- TEMPLATE 2: Girl/Egg-shell/Lion (6 idle, 5 run, 2+2 jump, 3 fall) fonctionne aussi avec les paire
static const FrameSegment humanoid_idle[] = {{0,0,0,4}};
static const FrameSegment humanoid_run[]  = {{1,0,1,4}};
static const FrameSegment humanoid_jump[] = {{1,5,1,5}, {2,0,2,1}};
static const FrameSegment humanoid_fall[] = {{2,2,2,4}};
static const FrameSegment humanoid_dash[] = {{2,5,2,5},{3,0,3,3}};
static const FrameSegment humanoid_hit[] = {{4,2,4,3}};

#define SEG_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))


// Structure interne pour mapper (type, animState) aux segments

typedef struct {
    const FrameSegment *idle;
    const FrameSegment *run;
    const FrameSegment *jump;
    const FrameSegment *fall;
    const FrameSegment *dash;
    const FrameSegment *hit;
} AnimSegmentSet;

static const AnimSegmentSet segmentDatabase[CHARACTER_COUNT] = {
 
    {.idle = caveman_idle, .run = caveman_run, .jump = caveman_jump, .fall = caveman_fall, .dash = caveman_dash, .hit = caveman_hit },
  
    {.idle = caveman_idle, .run = caveman_run, .jump = caveman_jump, .fall = caveman_fall, .dash = caveman_dash, .hit = caveman_hit},
  
    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit},
   
    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit},
  
    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit},
  
    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit},

    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit},
  
    {.idle = humanoid_idle, .run = humanoid_run, .jump = humanoid_jump, .fall = humanoid_fall, .dash = humanoid_dash, .hit = humanoid_hit}
};


// Accesseur des segments pour un personnage et un état d'anim
const FrameSegment *GetAnimSegments(int type, int animState, int *outCount) {
    if (outCount) *outCount = 0;
    if (type < 0 || type >= CHARACTER_COUNT) return NULL;

    const AnimSegmentSet *set = &segmentDatabase[type];
    const FrameSegment *segments = NULL;

    switch (animState) {
        case ANIM_IDLE:  segments = set->idle; break;
        case ANIM_RUN:   segments = set->run;  break;
        case ANIM_JUMP:  segments = set->jump; break;
        case ANIM_FALL:  segments = set->fall; break;
        case ANIM_DASH:  segments = set->dash; break;
        case ANIM_HIT:   segments = set->hit;  break;
        default:         return NULL;
    }

    if (segments && outCount) {
        // Calculer le nombre d'éléments dans le tableau
        // Grâce à la structure, on peut déterminer le count via SEG_COUNT sur le template
        if (segments == caveman_idle) *outCount = SEG_COUNT(caveman_idle);
        else if (segments == caveman_run) *outCount = SEG_COUNT(caveman_run);
        else if (segments == caveman_jump) *outCount = SEG_COUNT(caveman_jump);
        else if (segments == caveman_fall) *outCount = SEG_COUNT(caveman_fall);
        else if (segments == caveman_dash) *outCount = SEG_COUNT(caveman_dash);
        else if (segments == caveman_hit) *outCount = SEG_COUNT(caveman_hit);
        else if (segments == humanoid_idle) *outCount = SEG_COUNT(humanoid_idle);
        else if (segments == humanoid_run) *outCount = SEG_COUNT(humanoid_run);
        else if (segments == humanoid_jump) *outCount = SEG_COUNT(humanoid_jump);
        else if (segments == humanoid_fall) *outCount = SEG_COUNT(humanoid_fall);
        else if (segments == humanoid_dash) *outCount = SEG_COUNT(humanoid_dash);
        else if (segments == humanoid_hit) *outCount = SEG_COUNT(humanoid_hit);
    }

    return segments;
}

// Retourne le personnage "pair" (ex: caverman <-> caverman2)
CharacterType GetPairedCharacter(CharacterType type) {
    switch (type) {
        case CHARACTER_CAVERMAN: return CHARACTER_CAVERMAN2;
        case CHARACTER_CAVERMAN2: return CHARACTER_CAVERMAN;
        case CHARACTER_GIRL: return CHARACTER_GIRL2;
        case CHARACTER_GIRL2: return CHARACTER_GIRL;
        case CHARACTER_EGG_SHELL: return CHARACTER_EGG_SHELL_2;
        case CHARACTER_EGG_SHELL_2: return CHARACTER_EGG_SHELL;
        case CHARACTER_LION: return CHARACTER_LION2;
        case CHARACTER_LION2: return CHARACTER_LION;
        default: return type;
    }
}


// IMPLÉMENTATION DES FONCTIONS
SpriteMetadata GetSpriteMetadata(CharacterType type) {
    if (type < 0 || type >= CHARACTER_COUNT) {
        // Retourner le sprite par défaut en cas d'erreur
        return spriteDatabase[0];
    }
    return spriteDatabase[type];
}

SpriteMetadata *GetSpriteMetadataPtr(CharacterType type) {
    if (type < 0 || type >= CHARACTER_COUNT) {
        // Retourner un pointeur vers le sprite par défaut en cas d'erreur
        return &spriteDatabase[0];
    }
    return &spriteDatabase[type];
}


Rectangle GetFrameRectangle(SpriteMetadata metadata, int frameIndex, int rowIndex) {
    Rectangle rect = {0, 0, metadata.frameWidth, metadata.frameHeight};
    
    // Vérifier que les indices sont valides
    if (frameIndex < 0 || frameIndex >= metadata.framesPerRow) {
        frameIndex = 0;
    }
    if (rowIndex < 0 || rowIndex >= metadata.rowCount) {
        rowIndex = 0;
    }
    
    // Calculer les coordonnées X et Y dans le sprite sheet
    rect.x = frameIndex * metadata.frameWidth;
    rect.y = rowIndex * metadata.frameHeight;
    
    return rect;
}

