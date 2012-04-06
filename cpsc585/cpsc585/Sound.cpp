#include "Sound.h"


Sound::Sound(void)
{
	audio = NULL;
	mVoice = NULL;
	music = NULL;
	musicBuffer = NULL;
	laser = NULL;
	laserBuffer = NULL;
	laserBufferDetails = NULL;
	crash = NULL;
	crashBuffer = NULL;
	crashBufferDetails = NULL;
}


Sound::~Sound(void)
{
}

void Sound::initialize()
{
	/*XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	audio->CreateMasteringVoice(&mVoice);

	mVoice->SetVolume(1.0f);

	loadSound(music, "music.wav", musicBuffer);
	loadSound(laser, "laser.wav", laserBuffer);
	loadSound(crash, "crash.wav", crashBuffer);

	laser->SetVolume(0.15f);

	music->Start(0);*/
}

void Sound::shutdown()
{
	if (music)
	{
		music->Stop(0);
		music->DestroyVoice();
		music = NULL;
	}

	if (laser)
	{
		laser->DestroyVoice();
		laser = NULL;
	}

	if (crash)
	{
		crash->DestroyVoice();
		crash = NULL;
	}

	if (musicBuffer)
	{
		delete [] musicBuffer;
		musicBuffer = NULL;
	}

	if (laserBuffer)
	{
		delete [] laserBuffer;
		laserBuffer = NULL;
	}

	if (crashBuffer)
	{
		delete [] crashBuffer;
		crashBuffer = NULL;
	}

	if (laserBufferDetails)
	{
		delete laserBufferDetails;
		laserBufferDetails = NULL;
	}

	if (crashBufferDetails)
	{
		delete crashBufferDetails;
		crashBufferDetails = NULL;
	}

	if (mVoice)
	{
		mVoice->DestroyVoice();
		mVoice = NULL;
	}

	if (audio)
	{
		audio->Release();
		audio = NULL;
	}
}

void Sound::loadSound(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer)
{
	/*std::ifstream filestream(filename, std::ifstream::binary);
	
	_WIN32_FILE_ATTRIBUTE_DATA attrib;

	GetFileAttributesEx((const char*) filename.c_str(), GetFileExInfoStandard, &attrib);

	soundBuffer = new char[attrib.nFileSizeLow];
	
	int i = 0;
	while (!filestream.eof())
	{
		filestream.read(soundBuffer + i, 1024);
		i += 1024;
	}
	

	WAVEFORMATEX wfm;
	ZeroMemory(&wfm, sizeof(WAVEFORMATEX));

	wfm.wFormatTag = WAVE_FORMAT_PCM;
	wfm.nChannels = 1;
	wfm.nSamplesPerSec = 22050;
	wfm.wBitsPerSample = 16;
	wfm.nBlockAlign = wfm.nChannels * wfm.wBitsPerSample / 8;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;
	wfm.cbSize = 0;

	audio->CreateSourceVoice(&voice, &wfm, XAUDIO2_VOICE_NOPITCH, 1.0f);

	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	buffer.pAudioData = (BYTE*) soundBuffer;
	buffer.AudioBytes = attrib.nFileSizeLow;
	
	if (voice == music)
	{
		buffer.LoopCount = -1;
	}
	else if (voice == laser)
	{
		laserBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	else if (voice == crash)
	{
		crashBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	
	voice->SubmitSourceBuffer(&buffer);

	filestream.close();*/
}


void Sound::playLaser()
{
	//laser->FlushSourceBuffers();
	//laser->SubmitSourceBuffer(laserBufferDetails);
	//laser->Start(0);
}

void Sound::playCrash()
{
	//crash->FlushSourceBuffers();
	//crash->SubmitSourceBuffer(crashBufferDetails);
	//crash->Start(0);
}
