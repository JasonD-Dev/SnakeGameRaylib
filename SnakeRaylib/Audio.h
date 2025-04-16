#pragma once
#include "raylib.h"
#include <vector>

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
	void SetAllSFXVolume();
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleMute();
	void PlayNextMusic();
	float GetVolume() const;
	bool IsPlaying() const;

private:
    std::vector<Music> playlist;
	size_t currentMusicIndex = 0;
    Sound eatSFX;
    Sound dieSFX;
    bool muted = false;
	float volume;

};