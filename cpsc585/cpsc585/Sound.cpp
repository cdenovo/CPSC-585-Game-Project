#include "Sound.h"

Sound* Sound::sound = NULL;

Sound::Sound(void)
{
	initialized = false;
	playerEmitter = NULL;

	audio = NULL;
	mVoice = NULL;
	smSFX = NULL;
	smMusic = NULL;

	numClaimedEmitters = 0;
	emitters = NULL;

	dspSettings.pMatrixCoefficients = NULL;

	ingamemusic = NULL;
	ingamemusicBuffer = NULL;
	menumusic = NULL;
	menumusicBuffer = NULL;

	laserBufferDetails = NULL;
	crashBufferDetails = NULL;
	engineBufferDetails = NULL;
	boostBufferDetails = NULL;
	rocketBufferDetails = NULL;
	dropmineBufferDetails = NULL;
	scream1BufferDetails = NULL;
	scream2BufferDetails = NULL;
	scream3BufferDetails = NULL;
	explosionBufferDetails = NULL;
	carexplodeBufferDetails = NULL;
	beepBufferDetails = NULL;
	rocketlaunchBufferDetails = NULL;
	pickupBufferDetails = NULL;
	selectBufferDetails = NULL;
	shotgunBufferDetails = NULL;
	takenleadBufferDetails = NULL;
	lostleadBufferDetails = NULL;
	noammoBufferDetails = NULL;
	oneBufferDetails = NULL;
	twoBufferDetails = NULL;
	threeBufferDetails = NULL;

	laserWMABuffer = NULL;
	crashWMABuffer = NULL;
	engineWMABuffer = NULL;
	boostWMABuffer = NULL;
	rocketWMABuffer = NULL;
	dropmineWMABuffer = NULL;
	scream1WMABuffer = NULL;
	scream2WMABuffer = NULL;
	scream3WMABuffer = NULL;
	explosionWMABuffer = NULL;
	carexplodeWMABuffer = NULL;
	beepWMABuffer = NULL;
	rocketlaunchWMABuffer = NULL;
	pickupWMABuffer = NULL;
	selectWMABuffer = NULL;
	shotgunWMABuffer = NULL;
	takenleadWMABuffer = NULL;
	lostleadWMABuffer = NULL;
	noammoWMABuffer = NULL;
	oneWMABuffer = NULL;
	twoWMABuffer = NULL;
	threeWMABuffer = NULL;

	ingameMusicXMABuffer = NULL;
	menuMusicXMABuffer = NULL;

	sound = this;

	currentVoice = 0;
	maxVoices = 200;
	maxReservedVoices = 30;
	currentReservedVoice = 0;

	voiceBuffer = NULL;
	voiceBufferReserved = NULL;

	std::srand((unsigned int) time(NULL));
}

Sound::~Sound(void)
{
}

void Sound::initialize()
{
	HRESULT res;
	
	if (FAILED(res = XAudio2Create(&audio)))
	{
		// Failed to create XAudio2 engine instance
		initialized = false;
		
		return;
	}

	audio->GetDeviceDetails(0, &details);

	if (FAILED(res = audio->CreateMasteringVoice(&mVoice, details.OutputFormat.Format.nChannels)))
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
	// Music must have exactly TWO channels, sample rate of 48000 Hz
	audio->CreateSubmixVoice(&smMusic, 2, 48000);
	
	SFXSend.Flags = XAUDIO2_SEND_USEFILTER;
	SFXSend.pOutputVoice = smSFX;
	SFXSendList.SendCount = 1;
	SFXSendList.pSends = &SFXSend;
	
	musicSend.Flags = 0;
	musicSend.pOutputVoice = smMusic;
	musicSendList.SendCount = 1;
	musicSendList.pSends = &musicSend;

	// Set music volume HERE
	smMusic->SetVolume(0.4f);

	// Set sound effect volume HERE
	smSFX->SetVolume(1.0f);

	wfm = new WAVEFORMATEXTENSIBLE();
	ZeroMemory(wfm, sizeof(WAVEFORMATEXTENSIBLE));

	loadMusic(ingamemusic, "sounds/ingamemusic.xwm", ingamemusicBuffer, ingameMusicXMABuffer);
	loadMusic(menumusic, "sounds/menumusic.xwm", menumusicBuffer, menuMusicXMABuffer);
	loadSound(SFX_ROCKET, "sounds/rocket.xwm");
	loadSound(SFX_CRASH, "sounds/crash.xwm");
	loadSound(SFX_ENGINE, "sounds/engine.xwm");
	loadSound(SFX_BOOST, "sounds/boost.xwm");
	loadSound(SFX_LASER, "sounds/laser.xwm");
	loadSound(SFX_DROPMINE, "sounds/dropmine.xwm");
	loadSound(SFX_SCREAM1, "sounds/scream1.xwm");
	loadSound(SFX_SCREAM2, "sounds/scream2.xwm");
	loadSound(SFX_SCREAM3, "sounds/scream3.xwm");
	loadSound(SFX_CAREXPLODE, "sounds/carexplode.xwm");
	loadSound(SFX_EXPLOSION, "sounds/explosion.xwm");
	loadSound(SFX_BEEP, "sounds/beep.xwm");
	loadSound(SFX_ROCKETLAUNCH, "sounds/rocketlaunch.xwm");
	loadSound(SFX_PICKUP, "sounds/pickup.xwm");
	loadSound(SFX_SELECT, "sounds/select.xwm");
	loadSound(SFX_SHOTGUN, "sounds/shotgun.xwm");
	loadSound(SFX_TAKENLEAD, "sounds/takenlead.xwm");
	loadSound(SFX_LOSTLEAD, "sounds/lostlead.xwm");
	loadSound(SFX_NOAMMO, "sounds/noammo.xwm");
	loadSound(SFX_ONE, "sounds/one.xwm");
	loadSound(SFX_TWO, "sounds/two.xwm");
	loadSound(SFX_THREE, "sounds/three.xwm");
	

	// Now set up 3D audio
	X3DAudioInitialize(details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, audio3DHandle);

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
		emitters[i].InnerRadius = 6.0f;
		emitters[i].InnerRadiusAngle = X3DAUDIO_PI * 2.0f;

		emitters[i].CurveDistanceScaler = 8.0f;
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
		audio->CreateSourceVoice(&(voiceBuffer[i]), (WAVEFORMATEX*) wfm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
	}

	voiceBufferReserved = new IXAudio2SourceVoice*[maxReservedVoices];

	for (int i = 0; i < maxReservedVoices; i++)
	{
		voiceBufferReserved[i] = NULL;
		audio->CreateSourceVoice(&(voiceBufferReserved[i]), (WAVEFORMATEX*) wfm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
	}

	initialized = true;
	
	return;
}

void Sound::shutdown()
{
	if (ingamemusic)
	{
		ingamemusic->Stop(0);
		ingamemusic->DestroyVoice();
		ingamemusic = NULL;
	}

	if (menumusic)
	{
		menumusic->Stop(0);
		menumusic->DestroyVoice();
		menumusic = NULL;
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

	if (ingamemusicBuffer)
	{
		delete [] ingamemusicBuffer;
		ingamemusicBuffer = NULL;
	}

	if (menumusicBuffer)
	{
		delete [] menumusicBuffer;
		menumusicBuffer = NULL;
	}

	if (ingameMusicXMABuffer) {
		delete [] ingameMusicXMABuffer;
		ingameMusicXMABuffer = NULL;
	}

	if (menuMusicXMABuffer) {
		delete [] menuMusicXMABuffer;
		menuMusicXMABuffer = NULL;
	}



	if (laserBufferDetails)
	{
		delete [] laserBufferDetails->pAudioData;
		delete laserBufferDetails;
		laserBufferDetails = NULL;
	}

	if (crashBufferDetails)
	{
		delete [] crashBufferDetails->pAudioData;
		delete crashBufferDetails;
		crashBufferDetails = NULL;
	}

	if (engineBufferDetails)
	{
		delete [] engineBufferDetails->pAudioData;
		delete engineBufferDetails;
		engineBufferDetails = NULL;
	}

	if (boostBufferDetails)
	{
		delete [] boostBufferDetails->pAudioData;
		delete boostBufferDetails;
		boostBufferDetails = NULL;
	}

	if (rocketBufferDetails)
	{
		delete [] rocketBufferDetails->pAudioData;
		delete rocketBufferDetails;
		rocketBufferDetails = NULL;
	}

	if (dropmineBufferDetails)
	{
		delete [] dropmineBufferDetails->pAudioData;
		delete dropmineBufferDetails;
		dropmineBufferDetails = NULL;
	}

	if (scream1BufferDetails)
	{
		delete [] scream1BufferDetails->pAudioData;
		delete scream1BufferDetails;
		scream1BufferDetails = NULL;
	}

	if (scream2BufferDetails)
	{
		delete [] scream2BufferDetails->pAudioData;
		delete scream2BufferDetails;
		scream2BufferDetails = NULL;
	}

	if (scream3BufferDetails)
	{
		delete [] scream3BufferDetails->pAudioData;
		delete scream3BufferDetails;
		scream3BufferDetails = NULL;
	}

	if (explosionBufferDetails)
	{
		delete [] explosionBufferDetails->pAudioData;
		delete explosionBufferDetails;
		explosionBufferDetails = NULL;
	}

	if (carexplodeBufferDetails)
	{
		delete [] carexplodeBufferDetails->pAudioData;
		delete carexplodeBufferDetails;
		carexplodeBufferDetails = NULL;
	}

	if (beepBufferDetails)
	{
		delete [] beepBufferDetails->pAudioData;
		delete beepBufferDetails;
		beepBufferDetails = NULL;
	}
	
	if (rocketlaunchBufferDetails)
	{
		delete [] rocketlaunchBufferDetails->pAudioData;
		delete rocketlaunchBufferDetails;
		rocketlaunchBufferDetails = NULL;
	}

	if (pickupBufferDetails)
	{
		delete [] pickupBufferDetails->pAudioData;
		delete pickupBufferDetails;
		pickupBufferDetails = NULL;
	}

	if (selectBufferDetails)
	{
		delete [] selectBufferDetails->pAudioData;
		delete selectBufferDetails;
		selectBufferDetails = NULL;
	}

	if (shotgunBufferDetails)
	{
		delete [] shotgunBufferDetails->pAudioData;
		delete shotgunBufferDetails;
		shotgunBufferDetails = NULL;
	}

	if (takenleadBufferDetails)
	{
		delete [] takenleadBufferDetails->pAudioData;
		delete takenleadBufferDetails;
		takenleadBufferDetails = NULL;
	}

	if (lostleadBufferDetails)
	{
		delete [] lostleadBufferDetails->pAudioData;
		delete lostleadBufferDetails;
		lostleadBufferDetails = NULL;
	}

	if (noammoBufferDetails)
	{
		delete [] noammoBufferDetails->pAudioData;
		delete noammoBufferDetails;
		noammoBufferDetails = NULL;
	}

	if (oneBufferDetails)
	{
		delete [] oneBufferDetails->pAudioData;
		delete oneBufferDetails;
		oneBufferDetails = NULL;
	}

	if (twoBufferDetails)
	{
		delete [] twoBufferDetails->pAudioData;
		delete twoBufferDetails;
		twoBufferDetails = NULL;
	}

	if (threeBufferDetails)
	{
		delete [] threeBufferDetails->pAudioData;
		delete threeBufferDetails;
		threeBufferDetails = NULL;
	}



	// Now free up WMA buffers
	if (laserWMABuffer)
	{
		delete [] laserWMABuffer->pDecodedPacketCumulativeBytes;
		delete laserWMABuffer;
		laserWMABuffer = NULL;
	}

	if (crashWMABuffer)
	{
		delete [] crashWMABuffer->pDecodedPacketCumulativeBytes;
		delete crashWMABuffer;
		crashWMABuffer = NULL;
	}

	if (engineWMABuffer)
	{
		delete [] engineWMABuffer->pDecodedPacketCumulativeBytes;
		delete engineWMABuffer;
		engineWMABuffer = NULL;
	}

	if (boostWMABuffer)
	{
		delete [] boostWMABuffer->pDecodedPacketCumulativeBytes;
		delete boostWMABuffer;
		boostWMABuffer = NULL;
	}

	if (rocketWMABuffer)
	{
		delete [] rocketWMABuffer->pDecodedPacketCumulativeBytes;
		delete rocketWMABuffer;
		rocketWMABuffer = NULL;
	}

	if (dropmineWMABuffer)
	{
		delete [] dropmineWMABuffer->pDecodedPacketCumulativeBytes;
		delete dropmineWMABuffer;
		dropmineWMABuffer = NULL;
	}

	if (scream1WMABuffer)
	{
		delete [] scream1WMABuffer->pDecodedPacketCumulativeBytes;
		delete scream1WMABuffer;
		scream1WMABuffer = NULL;
	}

	if (scream2WMABuffer)
	{
		delete [] scream2WMABuffer->pDecodedPacketCumulativeBytes;
		delete scream2WMABuffer;
		scream2WMABuffer = NULL;
	}

	if (scream3WMABuffer)
	{
		delete [] scream3WMABuffer->pDecodedPacketCumulativeBytes;
		delete scream3WMABuffer;
		scream3WMABuffer = NULL;
	}

	if (explosionWMABuffer)
	{
		delete [] explosionWMABuffer->pDecodedPacketCumulativeBytes;
		delete explosionWMABuffer;
		explosionWMABuffer = NULL;
	}

	if (carexplodeWMABuffer)
	{
		delete [] carexplodeWMABuffer->pDecodedPacketCumulativeBytes;
		delete carexplodeWMABuffer;
		carexplodeWMABuffer = NULL;
	}

	if (beepWMABuffer)
	{
		delete [] beepWMABuffer->pDecodedPacketCumulativeBytes;
		delete beepWMABuffer;
		beepWMABuffer = NULL;
	}

	if (rocketlaunchWMABuffer)
	{
		delete [] rocketlaunchWMABuffer->pDecodedPacketCumulativeBytes;
		delete rocketlaunchWMABuffer;
		rocketlaunchWMABuffer = NULL;
	}

	if (pickupWMABuffer)
	{
		delete [] pickupWMABuffer->pDecodedPacketCumulativeBytes;
		delete pickupWMABuffer;
		pickupWMABuffer = NULL;
	}

	if (selectWMABuffer)
	{
		delete [] selectWMABuffer->pDecodedPacketCumulativeBytes;
		delete selectWMABuffer;
		selectWMABuffer = NULL;
	}

	if (shotgunWMABuffer)
	{
		delete [] shotgunWMABuffer->pDecodedPacketCumulativeBytes;
		delete shotgunWMABuffer;
		shotgunWMABuffer = NULL;
	}

	if (takenleadWMABuffer)
	{
		delete [] takenleadWMABuffer->pDecodedPacketCumulativeBytes;
		delete takenleadWMABuffer;
		takenleadWMABuffer = NULL;
	}

	if (lostleadWMABuffer)
	{
		delete [] lostleadWMABuffer->pDecodedPacketCumulativeBytes;
		delete lostleadWMABuffer;
		lostleadWMABuffer = NULL;
	}

	if (noammoWMABuffer)
	{
		delete [] noammoWMABuffer->pDecodedPacketCumulativeBytes;
		delete noammoWMABuffer;
		noammoWMABuffer = NULL;
	}

	if (oneWMABuffer)
	{
		delete [] oneWMABuffer->pDecodedPacketCumulativeBytes;
		delete oneWMABuffer;
		oneWMABuffer = NULL;
	}

	if (twoWMABuffer)
	{
		delete [] twoWMABuffer->pDecodedPacketCumulativeBytes;
		delete twoWMABuffer;
		twoWMABuffer = NULL;
	}

	if (threeWMABuffer)
	{
		delete [] threeWMABuffer->pDecodedPacketCumulativeBytes;
		delete threeWMABuffer;
		threeWMABuffer = NULL;
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

void Sound::loadSound(SoundEffect type, std::string filename)
{
	HANDLE fileHandle = CreateFile((const char*) filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	DWORD chunkSize = 0;
	DWORD chunkPos = 0;
	DWORD fileType = 0;


	FindChunk(fileHandle, 'FFIR', chunkSize, chunkPos);
	ReadChunkData(fileHandle, &fileType, sizeof(DWORD), chunkPos);

	FindChunk(fileHandle, ' tmf', chunkSize, chunkPos);
	ReadChunkData(fileHandle, wfm, chunkSize, chunkPos);

	// Read in audio data
	FindChunk(fileHandle, 'atad', chunkSize, chunkPos);
	BYTE* soundBuffer = new BYTE[chunkSize];
	ReadChunkData(fileHandle, soundBuffer, chunkSize, chunkPos);

	
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = soundBuffer;
	
	FindChunk(fileHandle, 'sdpd', chunkSize, chunkPos);

	switch (type) {
	case SFX_LASER:
		{
			laserBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			laserWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(laserWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			laserWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			laserWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_CRASH:
		{
			crashBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			crashWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(crashWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			crashWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			crashWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_ENGINE:
		{
			engineBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			engineWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(engineWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			engineWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			engineWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_BOOST:
		{
			boostBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			boostWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(boostWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			boostWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			boostWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_ROCKET:
		{
			rocketBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			rocketWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(rocketWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			rocketWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			rocketWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_DROPMINE:
		{
			dropmineBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			dropmineWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(dropmineWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			dropmineWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			dropmineWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_SCREAM1:
		{
			scream1BufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			scream1WMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(scream1WMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			scream1WMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			scream1WMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_SCREAM2:
		{
			scream2BufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			scream2WMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(scream2WMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			scream2WMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			scream2WMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_SCREAM3:
		{
			scream3BufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			scream3WMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(scream3WMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			scream3WMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			scream3WMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_CAREXPLODE:
		{
			carexplodeBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			carexplodeWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(carexplodeWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			carexplodeWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			carexplodeWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_EXPLOSION:
		{
			explosionBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			explosionWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(explosionWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			explosionWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			explosionWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_BEEP:
		{
			beepBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			beepWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(beepWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			beepWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			beepWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_ROCKETLAUNCH:
		{
			rocketlaunchBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			rocketlaunchWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(rocketlaunchWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			rocketlaunchWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			rocketlaunchWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_PICKUP:
		{
			pickupBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			pickupWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(pickupWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			pickupWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			pickupWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_SELECT:
		{
			selectBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			selectWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(selectWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			selectWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			selectWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_SHOTGUN:
		{
			shotgunBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			shotgunWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(shotgunWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			shotgunWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			shotgunWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_TAKENLEAD:
		{
			takenleadBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			takenleadWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(takenleadWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			takenleadWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			takenleadWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_LOSTLEAD:
		{
			lostleadBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			lostleadWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(lostleadWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			lostleadWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			lostleadWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_NOAMMO:
		{
			noammoBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			noammoWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(noammoWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			noammoWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			noammoWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_ONE:
		{
			oneBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			oneWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(oneWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			oneWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			oneWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_TWO:
		{
			twoBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			twoWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(twoWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			twoWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			twoWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	case SFX_THREE:
		{
			threeBufferDetails = new XAUDIO2_BUFFER(buffer);
			UINT32* xmaBuffer = (UINT32*) new BYTE[chunkSize];

			threeWMABuffer = new XAUDIO2_BUFFER_WMA();
			ZeroMemory(threeWMABuffer, sizeof(XAUDIO2_BUFFER_WMA));

			// Divide chunk size by sizeof(DWORD) and assign
			threeWMABuffer->PacketCount = chunkSize / 4;

			ReadChunkData(fileHandle, xmaBuffer, chunkSize, chunkPos);
			threeWMABuffer->pDecodedPacketCumulativeBytes = xmaBuffer;
			break;
		}
	default:
		break;
	}
	


	CloseHandle(fileHandle);
}

void Sound::loadMusic(IXAudio2SourceVoice* &voice, std::string filename, BYTE* &soundBuffer, UINT32* &xwmaBuffer)
{
	WAVEFORMATEXTENSIBLE wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEXTENSIBLE));
	
	HANDLE fileHandle = CreateFile((const char*) filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	DWORD chunkSize = 0;
	DWORD chunkPos = 0;
	DWORD fileType = 0;

	FindChunk(fileHandle, 'FFIR', chunkSize, chunkPos);
	ReadChunkData(fileHandle, &fileType, sizeof(DWORD), chunkPos);

	FindChunk(fileHandle, ' tmf', chunkSize, chunkPos);
	ReadChunkData(fileHandle, &wfx, chunkSize, chunkPos);


	// Read in audio data
	FindChunk(fileHandle, 'atad', chunkSize, chunkPos);
	soundBuffer = new BYTE[chunkSize];
	ReadChunkData(fileHandle, soundBuffer, chunkSize, chunkPos);

	
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = soundBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	
	XAUDIO2_BUFFER_WMA wmaBuffer;
	ZeroMemory(&wmaBuffer, sizeof(XAUDIO2_BUFFER_WMA));
	
	FindChunk(fileHandle, 'sdpd', chunkSize, chunkPos);
	// Divide chunk size by sizeof(DWORD) and assign
	wmaBuffer.PacketCount = chunkSize / 4;
	
	xwmaBuffer = (UINT32*) new BYTE[chunkSize];
	ReadChunkData(fileHandle, xwmaBuffer, chunkSize, chunkPos);
	wmaBuffer.pDecodedPacketCumulativeBytes = xwmaBuffer;
	
	audio->CreateSourceVoice(&voice, (WAVEFORMATEX*) &wfx, XAUDIO2_VOICE_USEFILTER, 2.0f, NULL, &musicSendList, NULL);
	voice->SubmitSourceBuffer(&buffer, &wmaBuffer);

	CloseHandle(fileHandle);
}

void Sound::playEngine(X3DAUDIO_EMITTER* emit, float freq, IXAudio2SourceVoice* engine)
{
	engine->FlushSourceBuffers();
	engine->SubmitSourceBuffer(engineBufferDetails, engineWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	engine->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	engine->SetFrequencyRatio(freq);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	engine->SetFilterParameters(&filterParameters);
	

	engine->Start();
	
}

void Sound::playRocket(X3DAUDIO_EMITTER* emit, IXAudio2SourceVoice* rocket)
{	
	rocket->FlushSourceBuffers();
	rocket->SubmitSourceBuffer(rocketBufferDetails, rocketWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	rocket->SetFrequencyRatio(3.0f * dspSettings.DopplerFactor);
	
	rocket->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	rocket->SetFilterParameters(&filterParameters);
	

	rocket->Start();
}

void Sound::playSoundEffect(SoundEffect effect, X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();
	
	switch (effect) {
	case SFX_LASER:
		{
			voice->SubmitSourceBuffer(laserBufferDetails, laserWMABuffer);
			break;
		}
	case SFX_CRASH:
		{
			voice->SubmitSourceBuffer(crashBufferDetails, crashWMABuffer);
			break;
		}
	case SFX_BOOST:
		{
			voice->SubmitSourceBuffer(boostBufferDetails, boostWMABuffer);
			break;
		}
	case SFX_DROPMINE:
		{
			voice->SubmitSourceBuffer(dropmineBufferDetails, dropmineWMABuffer);
			break;
		}
	case SFX_SCREAM:
		{
			// Now pick one of the three screams randomly
			int choice = std::rand() % 3;


			voice->SetVolume(2.0f);
			switch (choice) {
				case 0:	voice->SubmitSourceBuffer(scream1BufferDetails, scream1WMABuffer);
					break;
				case 1:	voice->SubmitSourceBuffer(scream2BufferDetails, scream2WMABuffer);
					break;
				case 2:	voice->SubmitSourceBuffer(scream3BufferDetails, scream3WMABuffer);
					break;
				default:
					voice->SubmitSourceBuffer(scream1BufferDetails, scream1WMABuffer);
			}

			break;
		}
	case SFX_CAREXPLODE:
		{
			voice->SetVolume(2.0f);
			voice->SubmitSourceBuffer(carexplodeBufferDetails, carexplodeWMABuffer);
			break;
		}
	case SFX_EXPLOSION:
		{
			voice->SubmitSourceBuffer(explosionBufferDetails, explosionWMABuffer);
			break;
		}
	case SFX_BEEP:
		{
			voice->SubmitSourceBuffer(beepBufferDetails, beepWMABuffer);
			break;
		}
	case SFX_ROCKETLAUNCH:
		{
			voice->SubmitSourceBuffer(rocketlaunchBufferDetails, rocketlaunchWMABuffer);
			break;
		}
	case SFX_PICKUP:
		{
			voice->SetVolume(2.0f);
			voice->SubmitSourceBuffer(pickupBufferDetails, pickupWMABuffer);
			break;
		}
	case SFX_SELECT:
		{
			voice->SubmitSourceBuffer(selectBufferDetails, selectWMABuffer);
			break;
		}
	case SFX_SHOTGUN:
		{
			voice->SetVolume(2.5f);
			voice->SubmitSourceBuffer(shotgunBufferDetails, shotgunWMABuffer);
			break;
		}
	case SFX_TAKENLEAD:
		{
			voice->SubmitSourceBuffer(takenleadBufferDetails, takenleadWMABuffer);
			break;
		}
	case SFX_LOSTLEAD:
		{
			voice->SubmitSourceBuffer(lostleadBufferDetails, lostleadWMABuffer);
			break;
		}
	case SFX_NOAMMO:
		{
			voice->SubmitSourceBuffer(noammoBufferDetails, noammoWMABuffer);
			break;
		}
	case SFX_ONE:
		{
			voice->SubmitSourceBuffer(oneBufferDetails, oneWMABuffer);
			break;
		}
	case SFX_TWO:
		{
			voice->SubmitSourceBuffer(twoBufferDetails, twoWMABuffer);
			break;
		}
	case SFX_THREE:
		{
			voice->SubmitSourceBuffer(threeBufferDetails, threeWMABuffer);
			break;
		}
	default:
		break;
	}




	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
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

		audio->CreateSourceVoice(&(voiceBuffer[maxVoices - 1]), (WAVEFORMATEX*) wfm,
			XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
		
		return voiceBuffer[maxVoices - 1];
	}
	
	voiceBuffer[currentVoice - 1]->FlushSourceBuffers();
	voiceBuffer[currentVoice - 1]->DestroyVoice();

	audio->CreateSourceVoice(&(voiceBuffer[currentVoice - 1]), (WAVEFORMATEX*) wfm,
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

		audio->CreateSourceVoice(&(voiceBufferReserved[maxReservedVoices - 1]), (WAVEFORMATEX*) wfm,
			XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);
		
		return voiceBufferReserved[maxReservedVoices - 1];
	}
	
	voiceBufferReserved[currentReservedVoice - 1]->FlushSourceBuffers();
	voiceBufferReserved[currentReservedVoice - 1]->DestroyVoice();

	audio->CreateSourceVoice(&(voiceBufferReserved[currentReservedVoice - 1]), (WAVEFORMATEX*) wfm,
		XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO, NULL, &SFXSendList, NULL);

	return voiceBufferReserved[currentReservedVoice - 1];
}

void Sound::playInGameMusic()
{
	if (menumusic)
		menumusic->Stop();

	ingamemusic->Start(0);
}

void Sound::playMenuMusic()
{
	if (ingamemusic)
		ingamemusic->Stop();

	menumusic->Start(0);
}













// Below methods are taken from MSDN XAudio2 reference

HRESULT Sound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
		case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );
            break;

        default:
            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
            return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;
		
        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
    
}

HRESULT Sound::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}