#include "Sound.h"

Sound* Sound::sound = NULL;

Sound::Sound(void)
{
	initialized = false;

	audio = NULL;
	mVoice = NULL;
	smSFX = NULL;
	smMusic = NULL;

	numClaimedEmitters = 0;
	emitters = NULL;

	dspSettings.pMatrixCoefficients = NULL;

	music = NULL;
	musicBuffer = NULL;
	laser = NULL;
	laserBuffer = NULL;
	laserBufferDetails = NULL;
	crash = NULL;
	crashBuffer = NULL;
	crashBufferDetails = NULL;
	engineBuffer = NULL;
	engineBufferDetails = NULL;
	boost = NULL;
	boostBuffer = NULL;
	boostBufferDetails = NULL;
	rocketBuffer = NULL;
	rocketBufferDetails = NULL;

	sound = this;

	currentVoice = 0;
	maxVoices = 200;
	maxReservedVoices = 30;
	currentReservedVoice = 0;

	voiceBuffer = NULL;
	voiceBufferReserved = NULL;
}


Sound::~Sound(void)
{
}

void Sound::initialize()
{
	HRESULT res;
	
	if (FAILED(res = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		// Failed to create XAudio2 engine instance
		initialized = false;
		
		return;
	}
	
	audio->GetDeviceDetails(NULL, &details);

	if (FAILED(res = audio->CreateMasteringVoice(&mVoice, details.OutputFormat.Format.nChannels, XAUDIO2_DEFAULT_SAMPLERATE)))
	{
		// Failed to create mastering voice
		initialized = false;

		return;
	}
	
	mVoice->SetVolume(1.0f);


	// Create Sound Effects submix voice (ALL sound effects pass through here)
	// A Sound Effect must have exactly ONE channel, sample rate of 44100 Hz
	audio->CreateSubmixVoice(&smSFX, details.OutputFormat.Format.nChannels, 44100);
	
	// Create Music submix voice (ALL music passes through here)
	// Music must have exactly TWO channels, sample rate of 44100 Hz
	audio->CreateSubmixVoice(&smMusic, details.OutputFormat.Format.nChannels, 44100);
	
	SFXSend.Flags = 0;
	SFXSend.pOutputVoice = smSFX;
	SFXSendList.SendCount = 1;
	SFXSendList.pSends = &SFXSend;
	
	musicSend.Flags = 0;
	musicSend.pOutputVoice = smMusic;
	musicSendList.SendCount = 1;
	musicSendList.pSends = &musicSend;

	// Set music volume HERE
	smMusic->SetVolume(0.2f);

	// Set sound effect volume HERE
	smSFX->SetVolume(2.0f);

	ZeroMemory(&wfm, sizeof(WAVEFORMATEX));

	wfm.wFormatTag = WAVE_FORMAT_PCM;
	wfm.nChannels = 1;
	wfm.nSamplesPerSec = 44100;
	wfm.wBitsPerSample = 16;
	wfm.nBlockAlign = wfm.nChannels * wfm.wBitsPerSample / 8;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;
	wfm.cbSize = 0;


	
	loadMusic(music, "sounds/music.wav", musicBuffer);
	loadSound(ROCKETSFX, "sounds/rocket.wav", rocketBuffer);
	loadSound(CRASHSFX, "sounds/crash.wav", crashBuffer);
	loadSound(ENGINESFX, "sounds/engine.wav", engineBuffer);
	loadSound(BOOSTSFX, "sounds/boost.wav", boostBuffer);
	loadSound(LASERSFX, "sounds/laser.wav", laserBuffer);



	float* matrix = new float[details.OutputFormat.Format.nChannels];
	for (int i = 0; i < details.OutputFormat.Format.nChannels; i++)
	{
		matrix[i] = 1.0f;
	}

	music->SetOutputMatrix(smMusic, 1,  details.OutputFormat.Format.nChannels, matrix);
	

	delete [] matrix;

	music->Start(0);


	// Now set up 3D audio
	X3DAudioInitialize(SPEAKER_STEREO, X3DAUDIO_SPEED_OF_SOUND, audio3DHandle);

	listener.pCone = NULL;
	X3DAUDIO_VECTOR vec;
	vec.x = 0.0f;
	vec.y = 0.0f;
	vec.z = 1.0f;

	listener.OrientFront = X3DAUDIO_VECTOR(vec);

	vec.y = 1.0f;
	vec.z = 0.0f;

	listener.OrientTop = X3DAUDIO_VECTOR(vec);

	listener.Position.x = 0.0f;
	listener.Position.y = 0.0f;
	listener.Position.z = 0.0f;

	listener.Velocity.x = 0.0f;
	listener.Velocity.y = 0.0f;
	listener.Velocity.z = 0.0f;


	// Now set up emitters

	emitters = new X3DAUDIO_EMITTER[NUM_EMITTERS];

	for (int i = 0; i < NUM_EMITTERS; i++)
	{
		ZeroMemory(&(emitters[i]), sizeof(emitters[i]));
		emitters[i].ChannelCount = 1;
		emitters[i].InnerRadius = 5.0f;
		emitters[i].InnerRadiusAngle = X3DAUDIO_PI * 2.0f;

		emitters[i].CurveDistanceScaler = 10.0f;
		emitters[i].DopplerScaler = 1.0f;
	}
	
	ZeroMemory(&dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));

	float* mat = new float[details.OutputFormat.Format.nChannels];
	dspSettings.SrcChannelCount = 1;
	dspSettings.DstChannelCount = details.OutputFormat.Format.nChannels;
	dspSettings.pMatrixCoefficients = mat;

	

	voiceBuffer = new IXAudio2SourceVoice*[maxVoices];

	for (int i = 0; i < maxVoices; i++)
	{
		voiceBuffer[i] = NULL;
		audio->CreateSourceVoice(&(voiceBuffer[i]), &wfm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
	}

	voiceBufferReserved = new IXAudio2SourceVoice*[maxReservedVoices];

	for (int i = 0; i < maxReservedVoices; i++)
	{
		voiceBufferReserved[i] = NULL;
		audio->CreateSourceVoice(&(voiceBufferReserved[i]), &wfm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
	}

	initialized = true;

	return;
}

void Sound::shutdown()
{
	if (music)
	{
		music->Stop(0);
		music->DestroyVoice();
		music = NULL;
	}

	if (voiceBuffer)
	{
		for (int i = 0; i < maxVoices; i++)
		{
			if (voiceBuffer[i])
				voiceBuffer[i]->DestroyVoice();
		}

		delete [] voiceBuffer;
	}

	if (voiceBufferReserved)
	{
		for (int i = 0; i < maxReservedVoices; i++)
		{
			if (voiceBufferReserved[i])
				voiceBufferReserved[i]->DestroyVoice();
		}

		delete [] voiceBufferReserved;
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

	if (engineBuffer)
	{
		delete [] engineBuffer;
		engineBuffer = NULL;
	}

	if (boostBuffer)
	{
		delete [] boostBuffer;
		boostBuffer = NULL;
	}

	if (rocketBuffer)
	{
		delete [] rocketBuffer;
		rocketBuffer = NULL;
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

	if (engineBufferDetails)
	{
		delete engineBufferDetails;
		engineBufferDetails = NULL;
	}

	if (boostBufferDetails)
	{
		delete boostBufferDetails;
		boostBufferDetails = NULL;
	}

	if (rocketBufferDetails)
	{
		delete rocketBufferDetails;
		rocketBufferDetails = NULL;
	}


	if (emitters)
	{
		delete [] emitters;
		emitters = NULL;
	}


	if (dspSettings.pMatrixCoefficients)
	{
		delete [] dspSettings.pMatrixCoefficients;
		dspSettings.pMatrixCoefficients = NULL;
	}


	if (smSFX)
	{
		smSFX->DestroyVoice();
		smSFX = NULL;
	}

	if (smMusic)
	{
		smMusic->DestroyVoice();
		smMusic = NULL;
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

void Sound::loadSound(SoundEffect type, std::string filename, char* &soundBuffer)
{
	std::ifstream filestream(filename, std::ifstream::binary);
	
	_WIN32_FILE_ATTRIBUTE_DATA attrib;

	GetFileAttributesEx((const char*) filename.c_str(), GetFileExInfoStandard, &attrib);

	soundBuffer = new char[attrib.nFileSizeLow];
	
	int i = 0;
	while (!filestream.eof())
	{
		filestream.read(soundBuffer + i, 1024);
		i += 1024;
	}
	
	
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	buffer.pAudioData = (BYTE*) soundBuffer;
	buffer.AudioBytes = attrib.nFileSizeLow;
	
	if (type == LASERSFX)
	{
		laserBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	else if (type == CRASHSFX)
	{
		crashBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	else if (type == ENGINESFX)
	{
		engineBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	else if (type == BOOSTSFX)
	{
		boostBufferDetails = new XAUDIO2_BUFFER(buffer);
	}
	else if (type ==ROCKETSFX)
	{
		rocketBufferDetails = new XAUDIO2_BUFFER(buffer);
	}

	filestream.close();
}

void Sound::loadMusic(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer)
{
	std::ifstream filestream(filename, std::ifstream::binary);
	
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

	audio->CreateSourceVoice(&voice, &wfm, XAUDIO2_VOICE_USEFILTER, 2.0f, NULL, &musicSendList, NULL);

	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	buffer.pAudioData = (BYTE*) soundBuffer;
	buffer.AudioBytes = attrib.nFileSizeLow;
	
	buffer.LoopCount = -1;
	
	voice->SubmitSourceBuffer(&buffer);

	filestream.close();
}


void Sound::playLaser(X3DAUDIO_EMITTER* emit)
{
	laser = getSFXVoice();

	laser->FlushSourceBuffers();
	laser->SubmitSourceBuffer(laserBufferDetails);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	laser->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	laser->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	laser->SetFilterParameters(&filterParameters);
	

	laser->Start(0);
}

void Sound::playCrash(X3DAUDIO_EMITTER* emit)
{
	crash = getSFXVoice();

	crash->SetVolume(1.5f);

	crash->FlushSourceBuffers();
	crash->SubmitSourceBuffer(crashBufferDetails);

	X3DAudioCalculate(audio3DHandle, &listener, emit, X3DAUDIO_CALCULATE_MATRIX, &dspSettings);
	
	crash->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	

	crash->Start(0);
}

void Sound::playEngine(X3DAUDIO_EMITTER* emit, float freq, IXAudio2SourceVoice* engine)
{
	engine->FlushSourceBuffers();
	engine->SubmitSourceBuffer(engineBufferDetails);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	engine->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	engine->SetFrequencyRatio(freq);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	engine->SetFilterParameters(&filterParameters);
	

	engine->Start(0);
}

void Sound::playRocket(X3DAUDIO_EMITTER* emit, IXAudio2SourceVoice* rocket)
{	
	rocket->FlushSourceBuffers();
	rocket->SubmitSourceBuffer(rocketBufferDetails);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	rocket->SetFrequencyRatio(dspSettings.DopplerFactor);
	
	rocket->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	rocket->SetFilterParameters(&filterParameters);
	

	rocket->Start(0);
}

void Sound::playBoost(X3DAUDIO_EMITTER* emit)
{
	boost = getSFXVoice();

	boost->FlushSourceBuffers();
	boost->SubmitSourceBuffer(boostBufferDetails);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	boost->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	boost->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	boost->SetFilterParameters(&filterParameters);
	
	boost->Start(0);
}



X3DAUDIO_EMITTER* Sound::getEmitter()
{
	_ASSERT(numClaimedEmitters < NUM_EMITTERS);
	numClaimedEmitters++;

	X3DAUDIO_EMITTER* emitter = &(emitters[numClaimedEmitters - 1]);
	emitter->Position.x = 0.0f;
	emitter->Position.y = 0.0f;
	emitter->Position.z = 0.0f;
	emitter->Velocity.x = 0.0f;
	emitter->Velocity.y = 0.0f;
	emitter->Velocity.z = 0.0f;

	return emitter;
}


void Sound::returnEmitter()
{
	if (numClaimedEmitters > 0)
	{
		numClaimedEmitters--;
	}
}


IXAudio2SourceVoice* Sound::getSFXVoice()
{
	currentVoice++;

	if (currentVoice == maxVoices)
	{
		currentVoice = 0;

		voiceBuffer[maxVoices - 1]->FlushSourceBuffers();
		voiceBuffer[maxVoices - 1]->DestroyVoice();

		audio->CreateSourceVoice(&(voiceBuffer[maxVoices - 1]), &wfm,
			XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
		
		return voiceBuffer[maxVoices - 1];
	}
	
	voiceBuffer[currentVoice - 1]->FlushSourceBuffers();
	voiceBuffer[currentVoice - 1]->DestroyVoice();

	audio->CreateSourceVoice(&(voiceBuffer[currentVoice - 1]), &wfm,
		XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);

	return voiceBuffer[currentVoice - 1];
}

IXAudio2SourceVoice* Sound::reserveSFXVoice()
{
	currentReservedVoice++;

	if (currentReservedVoice == maxReservedVoices)
	{
		currentReservedVoice = 0;

		voiceBufferReserved[maxReservedVoices - 1]->FlushSourceBuffers();
		voiceBufferReserved[maxReservedVoices - 1]->DestroyVoice();

		audio->CreateSourceVoice(&(voiceBufferReserved[maxReservedVoices - 1]), &wfm,
			XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
		
		return voiceBufferReserved[maxReservedVoices - 1];
	}
	
	voiceBufferReserved[currentReservedVoice - 1]->FlushSourceBuffers();
	voiceBufferReserved[currentReservedVoice - 1]->DestroyVoice();

	audio->CreateSourceVoice(&(voiceBufferReserved[currentReservedVoice - 1]), &wfm,
		XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);

	return voiceBufferReserved[currentReservedVoice - 1];
}