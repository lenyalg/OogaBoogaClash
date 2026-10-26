#include "sound_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Instance singleton
static SoundManager* g_soundManagerInstance = NULL;

SoundManager* InitSoundManager(void)
{
    // Retourner l'instance si déjà initialisée
    if (g_soundManagerInstance != NULL) {
        return g_soundManagerInstance;
    }

    g_soundManagerInstance = (SoundManager*)malloc(sizeof(SoundManager));
    if (g_soundManagerInstance == NULL) {
        return NULL;
    }

    g_soundManagerInstance->isInitialized = false;
    g_soundManagerInstance->isMusicPlaying = false;
    g_soundManagerInstance->hitSoundCount = 0;

    srand((unsigned int)time(NULL));

    // Chargement des sons de coup
    const char* hitSoundPaths[] = {
        "assets/sound/hit-1.wav",
        "assets/sound/hit-2.wav"
    };

    for (int i = 0; i < 2; i++) {
        g_soundManagerInstance->hitSounds[i] = LoadSound(hitSoundPaths[i]);
        if (g_soundManagerInstance->hitSounds[i].frameCount > 0) {
            g_soundManagerInstance->hitSoundCount++;
        }
    }

    // Chargement de la musique de fond
    const char* musicPath = "assets/music/theme-3.ogg";
    g_soundManagerInstance->backgroundMusic = LoadMusicStream(musicPath);

    g_soundManagerInstance->isInitialized = true;

    return g_soundManagerInstance;
}

SoundManager* GetSoundManager(void)
{
    return g_soundManagerInstance;
}

void PlayHitSound(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL || !manager->isInitialized || manager->hitSoundCount == 0) {
        return;
    }

    // Sélection aléatoire entre hit-1 et hit-2
    int randomIndex = rand() % manager->hitSoundCount;
    
    // Arrêter le son s'il joue déjà pour pouvoir le rejouer immédiatement
    if (IsSoundPlaying(manager->hitSounds[randomIndex])) {
        StopSound(manager->hitSounds[randomIndex]);
    }
    
    PlaySound(manager->hitSounds[randomIndex]);
}

void StartBackgroundMusic(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL || !manager->isInitialized || manager->backgroundMusic.frameCount == 0) {
        return;
    }

    manager->backgroundMusic.looping = true;
    PlayMusicStream(manager->backgroundMusic);
    manager->isMusicPlaying = true;
}

void PauseBackgroundMusic(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL || !manager->isMusicPlaying) {
        return;
    }
    PauseMusicStream(manager->backgroundMusic);
}

void ResumeBackgroundMusic(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL) {
        return;
    }
    ResumeMusicStream(manager->backgroundMusic);
}

void StopBackgroundMusic(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL || !manager->isMusicPlaying) {
        return;
    }
    StopMusicStream(manager->backgroundMusic);
    manager->isMusicPlaying = false;
}

void UpdateSoundManager(void)
{
    SoundManager* manager = GetSoundManager();
    if (manager == NULL || !manager->isInitialized || manager->backgroundMusic.frameCount == 0) {
        return;
    }
    // Maintient le flux de la musique
    UpdateMusicStream(manager->backgroundMusic);
}

void UnloadSoundManager(void)
{
    if (g_soundManagerInstance == NULL) {
        return;
    }

    // Libération des sons
    for (int i = 0; i < g_soundManagerInstance->hitSoundCount; i++) {
        UnloadSound(g_soundManagerInstance->hitSounds[i]);
    }

    // Libération de la musique
    if (g_soundManagerInstance->backgroundMusic.frameCount > 0) {
        UnloadMusicStream(g_soundManagerInstance->backgroundMusic);
    }

    free(g_soundManagerInstance);
    g_soundManagerInstance = NULL;
}
