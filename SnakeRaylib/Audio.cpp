#include "Audio.h"
//#include <iostream>

Audio::Audio() {
	InitAudioDevice();

	playlist.push_back(LoadMusicStream("resources/Music1.mp3"));
	playlist.push_back(LoadMusicStream("resources/Music2.mp3"));
	playlist.push_back(LoadMusicStream("resources/Music3.mp3"));
	playlist.push_back(LoadMusicStream("resources/Music4.mp3"));
	playlist.push_back(LoadMusicStream("resources/Music5.mp3"));
	
	for (Music& music : playlist) {
		music.looping = false;
	}

	eatSFX = LoadSound("resources/Eat.wav");
	dieSFX = LoadSound("resources/Death.wav");
	muted = false;

	volume = 0.2f; 
	SetMasterVolume(1.0f);
	SetAllSFXVolume();
	PlayMusic();
}



Audio::~Audio() {
	for (Music& music : playlist) {
		UnloadMusicStream(music);
	}

	UnloadSound(eatSFX);
	UnloadSound(dieSFX);
	CloseAudioDevice();
}

void Audio::Update() { 
	UpdateMusicStream(playlist[currentMusicIndex]);

	if (!IsMusicStreamPlaying(playlist[currentMusicIndex]) && !muted) {
		//std::cout << "Playing Next\n";
		PlayNextMusic();
	}
}

void Audio::PlayEat() { 
	PlaySound(eatSFX); 
}

void Audio::PlayDie() {
	PauseMusic();
	PlaySound(dieSFX);
}

void Audio::PlayMusic() {
	if (!playlist.empty()) {
		Music& music = playlist[currentMusicIndex];
		SetMusicVolume(music, muted ? 0.0f : volume);
		PlayMusicStream(music);
	}
}

void Audio::PauseMusic() { 
	//std::cout << currentMusicIndex << "\nPauseMusic\n";
	PauseMusicStream(playlist[currentMusicIndex]);
}
void Audio::ResumeMusic() { 
	ResumeMusicStream(playlist[currentMusicIndex]);
}

void Audio::SetAllSFXVolume() {
	SetSoundVolume(dieSFX, volume);
	SetSoundVolume(eatSFX, volume);
}

void Audio::IncreaseVolume() {
	if (volume < 1.0f) {
		volume += 0.05f;
		if (volume > 1.0f) volume = 1.0f;
		SetAllSFXVolume();
	}

	if (!muted) {
		SetMusicVolume(playlist[currentMusicIndex], volume);
	}
	
}

void Audio::DecreaseVolume() {
	if (volume > 0.0f) {
		volume -= 0.05f;
		if (volume < 0.0f) volume = 0.0f;
		SetAllSFXVolume();
	}

	if (!muted) {
		SetMusicVolume(playlist[currentMusicIndex], volume);
	}
}

void Audio::ToggleMute() {
	muted = !muted;

	if (muted) {
		PauseMusic();  
	}
	else {
		//std::cout << "Resume\n";
		ResumeMusic(); 
	}
}

void Audio::PlayNextMusic() {
	currentMusicIndex = (currentMusicIndex + 1) % playlist.size();
	PlayMusic();
}

float Audio::GetVolume() const {
	float temp = volume * 100;
	return  temp;
}

bool Audio::IsPlaying() const {
	return IsMusicStreamPlaying(playlist[currentMusicIndex]);
}
