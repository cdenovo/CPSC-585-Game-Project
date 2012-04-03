#pragma once
#pragma comment(lib, "X3DAudio.lib")

#include <XAudio2.h>
#include <X3DAudio.h>
#include <iostream>
#include <fstream>

enum SoundEffect { LASERSFX, CRASHSFX, ENGINESFX, BOOSTSFX, ROCKETSFX };

#define NUM_EMITTERS 500

class Sound
{
public:
	Sound(void);
	~Sound(void);
	void initialize();
	void shutdown();
	void playLaser(X3DAUDIO_EMITTER* emit);
	void playCrash(X3DAUDIO_EMITTER* emit);
	void playEngine(X3DAUDIO_EMITTER* emit, float freq, IXAudio2SourceVoice* engine);
	void playBoost(X3DAUDIO_EMITTER* emit);
	void returnEmitter();
	void playRocket(X3DAUDIO_EMITTER* emit, IXAudio2SourceVoice* rocket);

	IXAudio2SourceVoice* getSFXVoice();
	IXAudio2SourceVoice* reserveSFXVoice();

	bool initialized;

	X3DAUDIO_EMITTER* getEmitter();
	X3DAUDIO_LISTENER listener;

	static Sound* sound;

private:
	void loadSound(SoundEffect type, std::string filename, char* &soundBuffer);
	void loadMusic(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer);

	IXAudio2* audio;
	IXAudio2MasteringVoice* mVoice;

	IXAudio2SubmixVoice* smSFX;
	IXAudio2SubmixVoice* smMusic;

	XAUDIO2_SEND_DESCRIPTOR SFXSend;
	XAUDIO2_VOICE_SENDS SFXSendList;

	XAUDIO2_SEND_DESCRIPTOR musicSend;
	XAUDIO2_VOICE_SENDS musicSendList;


	X3DAUDIO_HANDLE audio3DHandle;

	X3DAUDIO_EMITTER* emitters;

	X3DAUDIO_DSP_SETTINGS dspSettings;

	int numClaimedEmitters;

	XAUDIO2_DEVICE_DETAILS details;


	IXAudio2SourceVoice* music;
	IXAudio2SourceVoice* laser;
	IXAudio2SourceVoice* crash;
	IXAudio2SourceVoice* boost;


	XAUDIO2_BUFFER* laserBufferDetails;
	XAUDIO2_BUFFER* crashBufferDetails;
	XAUDIO2_BUFFER* engineBufferDetails;
	XAUDIO2_BUFFER* boostBufferDetails;
	XAUDIO2_BUFFER* rocketBufferDetails;

	char* musicBuffer;
	char* laserBuffer;
	char* crashBuffer;
	char* engineBuffer;
	char* boostBuffer;
	char* rocketBuffer;


	int currentVoice;
	int maxVoices;

	int currentReservedVoice;
	int maxReservedVoices;

	IXAudio2SourceVoice** voiceBuffer;
	IXAudio2SourceVoice** voiceBufferReserved;

	WAVEFORMATEX wfm;
};
