#pragma once
#include "raylib.h"
#include <iostream>

class Audio {
public:
    Audio();
    ~Audio();

    void UpdateMusic();
    void PlayEatSoundEffect();
    void PlayDieSoundEffect();
    bool IsMusicPlaying() const;
    void PauseMusic();
    void ResumeMusic();
    void PlayMusic();
    float GetVolume();
    void IncreaseVolume();
    void DecreaseVolume();
    bool MuteMusic = false;

private:
    Music bgMusic = { 0 };
    Sound eatSoundEffect = { 0 };
    Sound dieSoundEffect = { 0 };
    float volume = 0.2f;
};