#pragma once
#include "raylib.h"

class Audio {
public:
    Audio();
    ~Audio();

	void Update();
	void PlayEat();
	void PlayDie();
	void PlayMusic();
	void PauseMusic();
	void ResumeMusic();
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleMute();
	float GetVolume() const;
	bool IsPlaying() const;
	bool IsMuted() const;

private:
    Music bgMusic;
    Sound eatSFX;
    Sound dieSFX;
    bool muted;
	float volume;

};