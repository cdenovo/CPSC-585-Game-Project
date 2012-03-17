#pragma once
#define WIN32_LEAN_AND_MEAN
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
	
private:
	void loadSound(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer);

	IXAudio2* audio;
	IXAudio2MasteringVoice* mVoice;
	IXAudio2SourceVoice* music;
	IXAudio2SourceVoice* laser;
	IXAudio2SourceVoice* crash;

	XAUDIO2_BUFFER* laserBufferDetails;
	XAUDIO2_BUFFER* crashBufferDetails;

	char* musicBuffer;
	char* laserBuffer;
	char* crashBuffer;
};
