#include "Audio.h"

Audio::Audio() {
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        std::cerr << "Audio Error: Audio Device not Initialised" << std::endl;
        return;
    }

    // Load sounds
    eatSoundEffect = LoadSound("wavFiles/Eat.wav");
    if (eatSoundEffect.frameCount == 0) {
        std::cerr << "Sound Error: Eat Sound not loaded" << std::endl;
    }

    dieSoundEffect = LoadSound("wavFiles/Death.wav");
    if (dieSoundEffect.frameCount == 0) {
        std::cerr << "Sound Error: Death Sound not loaded" << std::endl;
    }

    // Load music
    bgMusic = LoadMusicStream("wavFiles/BackgroundMusic.mp3");
    if (bgMusic.frameCount == 0) {
        std::cerr << "Music Error: Background Music not loaded" << std::endl;
    }

    SetSoundVolume(eatSoundEffect, volume + 0.3);
    SetSoundVolume(dieSoundEffect, volume);
    SetMusicVolume(bgMusic, volume - 0.1);
    PlayMusicStream(bgMusic);
}

Audio::~Audio() {
    UnloadSound(eatSoundEffect);
    UnloadSound(dieSoundEffect);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
}

void Audio::UpdateMusic() {
    UpdateMusicStream(bgMusic);
}

void Audio::PlayEatSoundEffect() {
    PlaySound(eatSoundEffect);
}

void Audio::PlayDieSoundEffect() {
    StopSound(eatSoundEffect);  
    PauseMusicStream(bgMusic);
    PlaySound(dieSoundEffect);
}

bool Audio::IsMusicPlaying() const {
    return IsMusicStreamPlaying(bgMusic); 
}

void Audio::PauseMusic() {
    if (IsMusicPlaying()) {
        PauseMusicStream(bgMusic);
    }
}

void Audio::ResumeMusic() {
    if (!IsMusicPlaying() && !MuteMusic) {
        ResumeMusicStream(bgMusic);
    }
}

void Audio::PlayMusic() {
    PlayMusicStream(bgMusic);
}
float Audio::GetVolume() {
    float temp = volume * 100;
    return temp;
}

void Audio::IncreaseVolume() {
    volume = (volume + 0.05f > 1.0f) ? 1.0f : volume + 0.05f;
    SetMusicVolume(bgMusic, volume);
    SetSoundVolume(eatSoundEffect, volume);
    SetSoundVolume(dieSoundEffect, volume);
}

void Audio::DecreaseVolume() {
    volume = (volume - 0.05f < 0.0f) ? 0.0f : volume - 0.05f;
    SetMusicVolume(bgMusic, volume);
    SetSoundVolume(eatSoundEffect, volume);
    SetSoundVolume(dieSoundEffect, volume);
}