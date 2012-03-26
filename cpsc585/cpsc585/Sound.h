#pragma once
#include <XAudio2.h>
#include <iostream>
#include <fstream>

class Sound
{
public:
	Sound(void);
	~Sound(void);
	void initialize();
	void shutdown();
	void playLaser();
	void playCrash();
	void playEngine(float freq);
	void playBoost();
	
private:
	void loadSound(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer);
	void loadMusic(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer);

	IXAudio2* audio;
	IXAudio2MasteringVoice* mVoice;
	IXAudio2SourceVoice* music;
	IXAudio2SourceVoice* laser;
	IXAudio2SourceVoice* crash;
	IXAudio2SourceVoice* engine;
	IXAudio2SourceVoice* boost;

	XAUDIO2_BUFFER* laserBufferDetails;
	XAUDIO2_BUFFER* crashBufferDetails;
	XAUDIO2_BUFFER* engineBufferDetails;
	XAUDIO2_BUFFER* boostBufferDetails;

	char* musicBuffer;
	char* laserBuffer;
	char* crashBuffer;
	char* engineBuffer;
	char* boostBuffer;
};
