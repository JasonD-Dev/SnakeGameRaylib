#include "Audio.h"

Audio::Audio() {
	InitAudioDevice();
	bgMusic = LoadMusicStream("resources/Music.mp3");
	eatSFX = LoadSound("resources/Eat.wav");
	dieSFX = LoadSound("resources/Death.wav");
	volume = 0.2f;
	muted = false;
	PlayMusicStream(bgMusic);
	SetMusicVolume(bgMusic, volume);
}

Audio::~Audio() {
	UnloadMusicStream(bgMusic);
	UnloadSound(eatSFX);
	UnloadSound(dieSFX);
	CloseAudioDevice();
}

void Audio::Update() {
	UpdateMusicStream(bgMusic);
}

void Audio::PlayEat() {
	PlaySound(eatSFX);
}

void Audio::PlayDie() {
	PauseMusic();
	PlaySound(dieSFX);
}

void Audio::PlayMusic() {
	PlayMusicStream(bgMusic);
}

void Audio::PauseMusic() {
	PauseMusicStream(bgMusic);
}

void Audio::ResumeMusic() {
	ResumeMusicStream(bgMusic);
}

void Audio::IncreaseVolume() {
	volume + .05f >= 1.0f ? volume = 1.0f : volume += .05f;
	SetMusicVolume(bgMusic, muted ? 0.0f : volume);
	SetSoundVolume(eatSFX, volume);
	SetSoundVolume(dieSFX, volume);

}

void Audio::DecreaseVolume() {
	volume - .05f <= 0.0f ? volume = 0.0f : volume -= .05f;
	SetMusicVolume(bgMusic, muted ? 0.0f : volume);
	SetSoundVolume(eatSFX, volume);
	SetSoundVolume(dieSFX, volume);

}

void Audio::ToggleMute() {
	muted = !muted;
	SetMusicVolume(bgMusic, muted ? 0.0f : volume);
}

float Audio::GetVolume() const {
	float temp = volume * 100;
	return  temp;
}

bool Audio::IsPlaying() const {
	return IsMusicStreamPlaying(bgMusic);
}

bool Audio::IsMuted() const {
	return muted;
}
