#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "raylib.h"
#include <stdbool.h>

// Structure Singleton pour la gestion audio
typedef struct {
    Sound hitSounds[2];
    int hitSoundCount;
    Music backgroundMusic;
    bool isInitialized;
    bool isMusicPlaying;
} SoundManager;

// Initialise le manager (appeler après InitAudioDevice)
SoundManager* InitSoundManager(void);

// Retourne l'instance singleton
SoundManager* GetSoundManager(void);

// Joue un son de coup aléatoire (hit-1 ou hit-2)
void PlayHitSound(void);

// Lance la musique en boucle (à appeler quand la partie démarre)
void StartBackgroundMusic(void);

// Contrôle de la musique
void PauseBackgroundMusic(void);
void ResumeBackgroundMusic(void);
void StopBackgroundMusic(void);

// Met à jour le flux musical (appeler chaque frame)
void UpdateSoundManager(void);

// Libère les ressources (appeler avant CloseAudioDevice)
void UnloadSoundManager(void);

#endif // SOUND_MANAGER_H
