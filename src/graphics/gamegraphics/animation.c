#include "animation.h"
#include "sprite_manager.h"
#include <stddef.h>  

// Remplit `outIndices` avec la séquence linéaire (row*framesPerRow + col)
// correspondant aux segments fournis. Retourne le nombre d'indices écrits.
static int fillIndicesFromSegments(const SpriteMetadata *meta, const FrameSegment *segments, int segCount, int *outIndices, int maxOut) {
    if (!meta || !outIndices) return 0;
    int fpsPerRow = meta->framesPerRow;
    int rowCount = meta->rowCount;
    int written = 0;

    for (int s = 0; s < segCount; ++s) {
        int rs = segments[s].startRow;
        int cs = segments[s].startCol;
        int re = segments[s].endRow;
        int ce = segments[s].endCol;

        if (rs < 0) rs = 0;
        if (re < 0) re = 0;
        if (rs >= rowCount) rs = rowCount - 1;
        if (re >= rowCount) re = rowCount - 1;
        if (cs < 0) cs = 0;
        if (ce < 0) ce = 0;
        if (cs >= fpsPerRow) cs = fpsPerRow - 1;
        if (ce >= fpsPerRow) ce = fpsPerRow - 1;

        for (int r = rs; r <= re; ++r) {
            int startC = (r == rs) ? cs : 0;
            int endC = (r == re) ? ce : (fpsPerRow - 1);
            for (int c = startC; c <= endC; ++c) {
                if (written >= maxOut) return written;
                outIndices[written++] = r * fpsPerRow + c;
            }
        }
    }

    return written;
}

static void GetAnimSegmentsForState(Character *player, const FrameSegment **outSegments, int *outSegCount) {
  if (player->state != ANIM_ATTACK) {
      *outSegments = GetAnimSegments((CharacterType)player->characterType, player->state, outSegCount);
      if (*outSegments == NULL || *outSegCount == 0) {
          static FrameSegment fallback = {0,0,0,5};
          *outSegments = &fallback;
          *outSegCount = 1;
      }
  } else {
    *outSegments = player->currentAttack.attackData->spriteSegments;
    *outSegCount = player->currentAttack.attackData->segCount;
  }
}

void UpdateAnimState(Character *player, bool isMoving) {
    // 1. Déterminer l'état (priorité à l'attaque pour ne pas être écrasée par le mouvement)
    int oldState = player->state;

    
    // Priorité a l'attaque
    if (player->isHited) {
        player->state = ANIM_HIT;
    } else if (player->isAttacking) {
        player->state = ANIM_ATTACK;
    } else if (player->isDashing) {
        player->state = ANIM_DASH;
    } else if (player->isOnGround) {
        if (isMoving) player->state = ANIM_RUN;
        else player->state = ANIM_IDLE;
    } else {
        if (player->velocityY < 0) player->state = ANIM_JUMP;
        else player->state = ANIM_FALL;
    }

    // 2. Construire la liste de frames pour l'animation en cours
    // Atention il faut que player->spriteMetadata soit défini
    if (!player->spriteMetadata) {
        CharacterType type = (CharacterType)player->characterType;
        player->spriteMetadata = GetSpriteMetadataPtr(type != 0 ? type : CHARACTER_CAVERMAN);
    }

    // Maximum raisonnable : framesPerRow * rowCount
    int maxFrames = 1;
    if (player->spriteMetadata) maxFrames = player->spriteMetadata->framesPerRow * player->spriteMetadata->rowCount;
    if (maxFrames <= 0) maxFrames = 64; // fallback
    int indices[maxFrames];
    int count = 0;

    // Obtenir les segments définis par le sprite manager pour ce personnage et cet état
    const FrameSegment *segments = NULL;
    int segCount = 0;
    GetAnimSegmentsForState(player, &segments, &segCount);
    
    count = fillIndicesFromSegments(player->spriteMetadata, segments, segCount, indices, maxFrames);
   
    if (count <= 0) return;

    // Si l'état a changé, réinitialiser l'animation
    if (oldState != (int)player->state) {
      player->currentFrame = 0;
      player->framesCounter = 0;
    }

    // 3. Avancer les frames à vitesse constante (ANIMATION_SPEED défini en fps)
    player->framesCounter++;
    int frameThreshold;
    switch (player->state)
    {
    case ANIM_DASH:
      frameThreshold = (60/ANIMATION_SPEED_DASH);
      break;

    case ANIM_ATTACK:
      
      return;
    
    default:
      frameThreshold = (60/ANIMATION_SPEED);
      break;
    }
    if (frameThreshold <= 0) frameThreshold = 1;
    if (player->framesCounter >= frameThreshold) {
        player->framesCounter = 0;
        player->currentFrame++;
        if (player->currentFrame >= count) player->currentFrame = 0;
    }
}

void DrawCharacter(Character *player, Texture2D sprite, Color tint) {
    // Vérifier que les métadonnées sont présentes
    if (player->spriteMetadata == 0) {
        // Utiliser les métadonnées par défaut (caveman) si non initialisé
        player->spriteMetadata = GetSpriteMetadataPtr(CHARACTER_CAVERMAN);
    }

    Rectangle sourceRec = {0, 0, 0, 0}; 
    
    // --- 1. RÉCUPÉRER LES DIMENSIONS DU SPRITE ---
    float frameWidth = player->spriteMetadata->frameWidth;
    float frameHeight = player->spriteMetadata->frameHeight;

    // Construire la même liste de frames que dans UpdateAnimState pour retrouver la frame courante
    const FrameSegment *segments2 = NULL;
    int segCount2 = 0;
    GetAnimSegmentsForState(player, &segments2, &segCount2);

    int maxFrames2 = player->spriteMetadata->framesPerRow * player->spriteMetadata->rowCount;
    if (maxFrames2 <= 0) maxFrames2 = 64;
    int indices2[maxFrames2];
    int count2 = fillIndicesFromSegments(player->spriteMetadata, segments2, segCount2, indices2, maxFrames2);
    if (count2 <= 0) return;

    // Clamp currentFrame
    int frameIndex;
    if (player->state == ANIM_ATTACK) {
        frameIndex = player->currentAttack.currentSpriteIndex;
    } else {
        frameIndex = player->currentFrame % count2;
    }
    int linear = indices2[frameIndex];
    int framesPerRow = player->spriteMetadata->framesPerRow;
    int srcRow = linear / framesPerRow;
    int srcCol = linear % framesPerRow;

    // Utiliser la fonction générique pour obtenir la position du frame
    sourceRec = GetFrameRectangle(*player->spriteMetadata, srcCol, srcRow);
    sourceRec.width = frameWidth;
    sourceRec.height = frameHeight;

    // Gestion du regard à gauche (miroir)
    if (!player->isFacingRight) sourceRec.width *= -1;

    // --- 3. CENTRAGE DE L'IMAGE SUR LA HITBOX ---
    Rectangle destRec;
    destRec.width = frameWidth;
    destRec.height = frameHeight;
     
    // Centrage horizontal
    destRec.x = player->spriteX - (frameWidth - player->width) / 2;
    
    // Alignement vertical : on aligne le bas de l'image avec le bas de la hitbox
    // On applique en plus un offset configurable depuis les métadonnées du sprite
    destRec.y = player->spriteY - (frameHeight - player->height) + player->spriteMetadata->anchorOffsetY;

    Vector2 origin = { 0, 0 };
    DrawTexturePro(sprite, sourceRec, destRec, origin, 0.0f, tint);
}