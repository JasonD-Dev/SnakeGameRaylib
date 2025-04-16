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
	SetSoundVolume(eatSFX, volume + 0.1f >= 1.0f ? 1.0f : volume + 0.1f);
	PlaySound(eatSFX);
}

void Audio::PlayDie() {
	SetSoundVolume(dieSFX, volume);
	PlaySound(dieSFX);
	PauseMusic();
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
}

void Audio::DecreaseVolume() {
	volume - .05f <= 0.0f ? volume = 0.0f : volume -= .05f;
	SetMusicVolume(bgMusic, muted ? 0.0f : volume);
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
