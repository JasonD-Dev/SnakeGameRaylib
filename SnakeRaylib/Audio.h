#pragma once
#include "raylib.h"
#include <iostream>

class Audio {
public:
    Music bgMusic = { 0 };
    Audio();
    ~Audio();

    void UpdateMusic();
    void PlayEatSoundEffect();
    void PlayDieSoundEffect();
    bool IsMusicPlaying() const;
    void PauseMusic();
    void ResumeMusic();
    void PlayMusic();
    void IncreaseVolume();
    void DecreaseVolume();

private:
    Sound eatSoundEffect = { 0 };
    Sound dieSoundEffect = { 0 };
    float volume = 0.2f;
};