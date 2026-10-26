#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "raylib.h"

// STRUCTURE GÉNÉRIQUE POUR LES SPRITES
typedef struct {
    char name[64];           // Nom perso
    float frameWidth;        // Largeur d'une frame en pixels
    float frameHeight;       // Hauteur d'une frame en pixels
    int framesPerRow;        // Nombre de frames par ligne (colonnes)
    int rowCount;            // Nombre de lignes
    float spriteWidth;       // Largeur totale du sprite sheet
    float spriteHeight;      // Hauteur totale du sprite sheet
    float anchorOffsetY;     // Décalage vertical pour aligner les pieds
} SpriteMetadata;

// Petite structure décrivant un segment de frames (startRow,startCol -> endRow,endCol)
typedef struct {
    int startRow;
    int startCol;
    int endRow;
    int endCol;
} FrameSegment;

// Retourne un pointeur vers un tableau de FrameSegment décrivant l'animation
// pour le personnage `type` et l'état `animState` (0=IDLE,1=RUN,2=JUMP,3=FALL).
// *outCount reçoit le nombre d'éléments dans le tableau. Retourne NULL si aucun segment.
const FrameSegment *GetAnimSegments(int type, int animState, int *outCount);

// Retourne le CharacterType pair (ex: caverman <-> caverman2). Si aucun, retourne le même.
// Prototype ajouté après l'enum CharacterType (pour éviter l'utilisation avant déclaration)


// ÉNUMÉRATION DES PERSONNAGES DISPONIBLES
typedef enum {
    CHARACTER_CAVERMAN = 0,
    CHARACTER_CAVERMAN2 = 1,
    CHARACTER_GIRL = 2,
    CHARACTER_GIRL2 = 3,
    CHARACTER_EGG_SHELL = 4,
    CHARACTER_EGG_SHELL_2 = 5,
    CHARACTER_LION = 6,
    CHARACTER_LION2 = 7,
    CHARACTER_COUNT = 8
} CharacterType;

// Retourne le CharacterType pair (ex: caverman <-> caverman2). Si aucun, retourne le même.
CharacterType GetPairedCharacter(CharacterType type);


// FONCTION POUR OBTENIR LES MÉTADONNÉES D'UN SPRITE
// Retourne les métadonnées du sprite (dimensions, nombre de frames, etc.)
SpriteMetadata GetSpriteMetadata(CharacterType type);

// FONCTION POUR OBTENIR UN POINTEUR VERS LES MÉTADONNÉES
// Retourne un pointeur vers les métadonnées du sprite
SpriteMetadata *GetSpriteMetadataPtr(CharacterType type);

// FONCTION POUR CALCULER LA POSITION D'UNE FRAME
// Prends l'index de la frame (0 à framesPerRow-1), l'index de la ligne (0 à rowCount-1)
// Retourne un rectangle avec les coordonnées de la frame
Rectangle GetFrameRectangle(SpriteMetadata metadata, int frameIndex, int rowIndex);

#endif

