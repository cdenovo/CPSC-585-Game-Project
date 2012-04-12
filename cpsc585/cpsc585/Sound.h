#pragma once
#pragma comment(lib, "X3DAudio.lib")

#include <XAudio2.h>
#include <X3DAudio.h>
#include <iostream>
#include <fstream>
#include <time.h>

enum SoundEffect { LASERSFX, CRASHSFX, ENGINESFX, BOOSTSFX, ROCKETSFX, DROPMINESFX,
	SCREAM1SFX, SCREAM2SFX, SCREAM3SFX, CAREXPLODESFX, EXPLOSIONSFX, BEEPSFX,
	ROCKETLAUNCHSFX, PICKUPSFX };

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
	void playDropMine(X3DAUDIO_EMITTER* emit);
	void playScream(X3DAUDIO_EMITTER* emit);
	void playExplosion(X3DAUDIO_EMITTER* emit);
	void playCarExplode(X3DAUDIO_EMITTER* emit);
	void playBeep(X3DAUDIO_EMITTER* emit);
	void playRocketLaunch(X3DAUDIO_EMITTER* emit);
	void playPickup(X3DAUDIO_EMITTER* emit);
	void returnEmitter();
	void playRocket(X3DAUDIO_EMITTER* emit, IXAudio2SourceVoice* rocket);
	void playInGameMusic();
	void playMenuMusic();

	IXAudio2SourceVoice* getSFXVoice();
	IXAudio2SourceVoice* reserveSFXVoice();

	bool initialized;

	X3DAUDIO_EMITTER* getEmitter();
	X3DAUDIO_LISTENER listener;

	static Sound* sound;

private:
	// Some methods from MSDN
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);

	void loadSound(SoundEffect type, std::string filename, char* &soundBuffer);
	void loadMusic(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer, UINT32* &xwmaBuffer);

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


	IXAudio2SourceVoice* ingamemusic;
	IXAudio2SourceVoice* menumusic;


	XAUDIO2_BUFFER* laserBufferDetails;
	XAUDIO2_BUFFER* crashBufferDetails;
	XAUDIO2_BUFFER* engineBufferDetails;
	XAUDIO2_BUFFER* boostBufferDetails;
	XAUDIO2_BUFFER* rocketBufferDetails;
	XAUDIO2_BUFFER* dropmineBufferDetails;
	XAUDIO2_BUFFER* scream1BufferDetails;
	XAUDIO2_BUFFER* scream2BufferDetails;
	XAUDIO2_BUFFER* scream3BufferDetails;
	XAUDIO2_BUFFER* explosionBufferDetails;
	XAUDIO2_BUFFER* carexplodeBufferDetails;
	XAUDIO2_BUFFER* beepBufferDetails;
	XAUDIO2_BUFFER* rocketlaunchBufferDetails;
	XAUDIO2_BUFFER* pickupBufferDetails;

	XAUDIO2_BUFFER_WMA* laserWMABuffer;
	XAUDIO2_BUFFER_WMA* crashWMABuffer;
	XAUDIO2_BUFFER_WMA* engineWMABuffer;
	XAUDIO2_BUFFER_WMA* boostWMABuffer;
	XAUDIO2_BUFFER_WMA* rocketWMABuffer;
	XAUDIO2_BUFFER_WMA* dropmineWMABuffer;
	XAUDIO2_BUFFER_WMA* scream1WMABuffer;
	XAUDIO2_BUFFER_WMA* scream2WMABuffer;
	XAUDIO2_BUFFER_WMA* scream3WMABuffer;
	XAUDIO2_BUFFER_WMA* explosionWMABuffer;
	XAUDIO2_BUFFER_WMA* carexplodeWMABuffer;
	XAUDIO2_BUFFER_WMA* beepWMABuffer;
	XAUDIO2_BUFFER_WMA* rocketlaunchWMABuffer;
	XAUDIO2_BUFFER_WMA* pickupWMABuffer;

	char* ingamemusicBuffer;
	char* menumusicBuffer;
	char* laserBuffer;
	char* crashBuffer;
	char* engineBuffer;
	char* boostBuffer;
	char* rocketBuffer;
	char* dropmineBuffer;
	char* scream1Buffer;
	char* scream2Buffer;
	char* scream3Buffer;
	char* carexplodeBuffer;
	char* explosionBuffer;
	char* beepBuffer;
	char* rocketlaunchBuffer;
	char* pickupBuffer;


	int currentVoice;
	int maxVoices;

	int currentReservedVoice;
	int maxReservedVoices;

	IXAudio2SourceVoice** voiceBuffer;
	IXAudio2SourceVoice** voiceBufferReserved;

	WAVEFORMATEXTENSIBLE* wfm;

	UINT32* ingameMusicXMABuffer;
	UINT32* menuMusicXMABuffer;
};
