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

	laserBuffer = NULL;
	laserBufferDetails = NULL;
	crashBuffer = NULL;
	crashBufferDetails = NULL;
	engineBuffer = NULL;
	engineBufferDetails = NULL;
	boostBuffer = NULL;
	boostBufferDetails = NULL;
	rocketBuffer = NULL;
	rocketBufferDetails = NULL;
	dropmineBuffer = NULL;
	dropmineBufferDetails = NULL;
	scream1Buffer = NULL;
	scream1BufferDetails = NULL;
	scream2Buffer = NULL;
	scream2BufferDetails = NULL;
	scream3Buffer = NULL;
	scream3BufferDetails = NULL;
	explosionBuffer = NULL;
	explosionBufferDetails = NULL;
	carexplodeBuffer = NULL;
	carexplodeBufferDetails = NULL;
	beepBuffer = NULL;
	beepBufferDetails = NULL;
	rocketlaunchBuffer = NULL;
	rocketlaunchBufferDetails = NULL;
	pickupBuffer = NULL;
	pickupBufferDetails = NULL;
	selectBuffer = NULL;
	selectBufferDetails = NULL;
	shotgunBuffer = NULL;
	shotgunBufferDetails = NULL;
	takenleadBuffer = NULL;
	takenleadBufferDetails = NULL;
	lostleadBuffer = NULL;
	lostleadBufferDetails = NULL;
	noammoBuffer = NULL;
	noammoBufferDetails = NULL;
	oneBuffer = NULL;
	oneBufferDetails = NULL;
	twoBuffer = NULL;
	twoBufferDetails = NULL;
	threeBuffer = NULL;
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
	loadSound(ROCKETSFX, "sounds/rocket.xwm", rocketBuffer);
	loadSound(CRASHSFX, "sounds/crash.xwm", crashBuffer);
	loadSound(ENGINESFX, "sounds/engine.xwm", engineBuffer);
	loadSound(BOOSTSFX, "sounds/boost.xwm", boostBuffer);
	loadSound(LASERSFX, "sounds/laser.xwm", laserBuffer);
	loadSound(DROPMINESFX, "sounds/dropmine.xwm", dropmineBuffer);
	loadSound(SCREAM1SFX, "sounds/scream1.xwm", scream1Buffer);
	loadSound(SCREAM2SFX, "sounds/scream2.xwm", scream2Buffer);
	loadSound(SCREAM3SFX, "sounds/scream3.xwm", scream3Buffer);
	loadSound(CAREXPLODESFX, "sounds/carexplode.xwm", carexplodeBuffer);
	loadSound(EXPLOSIONSFX, "sounds/explosion.xwm", explosionBuffer);
	loadSound(BEEPSFX, "sounds/beep.xwm", beepBuffer);
	loadSound(ROCKETLAUNCHSFX, "sounds/rocketlaunch.xwm", rocketlaunchBuffer);
	loadSound(PICKUPSFX, "sounds/pickup.xwm", pickupBuffer);
	loadSound(SELECTSFX, "sounds/select.xwm", selectBuffer);
	loadSound(SHOTGUNSFX, "sounds/shotgun.xwm", shotgunBuffer);
	loadSound(TAKENLEADSFX, "sounds/takenlead.xwm", takenleadBuffer);
	loadSound(LOSTLEADSFX, "sounds/lostlead.xwm", lostleadBuffer);
	loadSound(NOAMMOSFX, "sounds/noammo.xwm", noammoBuffer);
	loadSound(ONESFX, "sounds/one.xwm", oneBuffer);
	loadSound(TWOSFX, "sounds/two.xwm", twoBuffer);
	loadSound(THREESFX, "sounds/three.xwm", threeBuffer);
	

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

	if (dropmineBuffer)
	{
		delete [] dropmineBuffer;
		dropmineBuffer = NULL;
	}

	if (scream1Buffer)
	{
		delete [] scream1Buffer;
		scream1Buffer = NULL;
	}

	if (scream2Buffer)
	{
		delete [] scream2Buffer;
		scream2Buffer = NULL;
	}

	if (scream3Buffer)
	{
		delete [] scream3Buffer;
		scream3Buffer = NULL;
	}

	if (explosionBuffer)
	{
		delete [] explosionBuffer;
		explosionBuffer = NULL;
	}

	if (carexplodeBuffer)
	{
		delete [] carexplodeBuffer;
		carexplodeBuffer = NULL;
	}

	if (beepBuffer)
	{
		delete [] beepBuffer;
		beepBuffer = NULL;
	}
	
	if (rocketlaunchBuffer)
	{
		delete [] rocketlaunchBuffer;
		rocketlaunchBuffer = NULL;
	}

	if (pickupBuffer)
	{
		delete [] pickupBuffer;
		pickupBuffer = NULL;
	}

	if (selectBuffer)
	{
		delete [] selectBuffer;
		selectBuffer = NULL;
	}

	if (shotgunBuffer)
	{
		delete [] shotgunBuffer;
		shotgunBuffer = NULL;
	}

	if (takenleadBuffer)
	{
		delete [] takenleadBuffer;
		takenleadBuffer = NULL;
	}

	if (lostleadBuffer)
	{
		delete [] lostleadBuffer;
		lostleadBuffer = NULL;
	}

	if (noammoBuffer)
	{
		delete [] noammoBuffer;
		noammoBuffer = NULL;
	}

	if (oneBuffer)
	{
		delete [] oneBuffer;
		oneBuffer = NULL;
	}

	if (twoBuffer)
	{
		delete [] twoBuffer;
		twoBuffer = NULL;
	}

	if (threeBuffer)
	{
		delete [] threeBuffer;
		threeBuffer = NULL;
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

	if (dropmineBufferDetails)
	{
		delete dropmineBufferDetails;
		dropmineBufferDetails = NULL;
	}

	if (scream1BufferDetails)
	{
		delete scream1BufferDetails;
		scream1BufferDetails = NULL;
	}

	if (scream2BufferDetails)
	{
		delete scream2BufferDetails;
		scream2BufferDetails = NULL;
	}

	if (scream3BufferDetails)
	{
		delete scream3BufferDetails;
		scream3BufferDetails = NULL;
	}

	if (explosionBufferDetails)
	{
		delete explosionBufferDetails;
		explosionBufferDetails = NULL;
	}

	if (carexplodeBufferDetails)
	{
		delete carexplodeBufferDetails;
		carexplodeBufferDetails = NULL;
	}

	if (beepBufferDetails)
	{
		delete beepBufferDetails;
		beepBufferDetails = NULL;
	}
	
	if (rocketlaunchBufferDetails)
	{
		delete rocketlaunchBufferDetails;
		rocketlaunchBufferDetails = NULL;
	}

	if (pickupBufferDetails)
	{
		delete pickupBufferDetails;
		pickupBufferDetails = NULL;
	}

	if (selectBufferDetails)
	{
		delete selectBufferDetails;
		selectBufferDetails = NULL;
	}

	if (shotgunBufferDetails)
	{
		delete shotgunBufferDetails;
		shotgunBufferDetails = NULL;
	}

	if (takenleadBufferDetails)
	{
		delete takenleadBufferDetails;
		takenleadBufferDetails = NULL;
	}

	if (lostleadBufferDetails)
	{
		delete lostleadBufferDetails;
		lostleadBufferDetails = NULL;
	}

	if (noammoBufferDetails)
	{
		delete noammoBufferDetails;
		noammoBufferDetails = NULL;
	}

	if (oneBufferDetails)
	{
		delete oneBufferDetails;
		oneBufferDetails = NULL;
	}

	if (twoBufferDetails)
	{
		delete twoBufferDetails;
		twoBufferDetails = NULL;
	}

	if (threeBufferDetails)
	{
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

void Sound::loadSound(SoundEffect type, std::string filename, char* &soundBuffer)
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
	soundBuffer = (char*) new BYTE[chunkSize];
	ReadChunkData(fileHandle, soundBuffer, chunkSize, chunkPos);

	
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = (BYTE*) soundBuffer;
	
	FindChunk(fileHandle, 'sdpd', chunkSize, chunkPos);

	switch (type) {
	case LASERSFX:
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
	case CRASHSFX:
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
	case ENGINESFX:
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
	case BOOSTSFX:
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
	case ROCKETSFX:
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
	case DROPMINESFX:
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
	case SCREAM1SFX:
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
	case SCREAM2SFX:
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
	case SCREAM3SFX:
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
	case CAREXPLODESFX:
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
	case EXPLOSIONSFX:
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
	case BEEPSFX:
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
	case ROCKETLAUNCHSFX:
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
	case PICKUPSFX:
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
	case SELECTSFX:
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
	case SHOTGUNSFX:
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
	case TAKENLEADSFX:
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
	case LOSTLEADSFX:
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
	case NOAMMOSFX:
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
	case ONESFX:
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
	case TWOSFX:
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
	case THREESFX:
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

void Sound::loadMusic(IXAudio2SourceVoice* &voice, std::string filename, char* &soundBuffer, UINT32* &xwmaBuffer)
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
	soundBuffer = (char*) new BYTE[chunkSize];
	ReadChunkData(fileHandle, soundBuffer, chunkSize, chunkPos);

	
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = (BYTE*) soundBuffer;
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

void Sound::playLaser(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* laser = getSFXVoice();

	laser->FlushSourceBuffers();
	laser->SubmitSourceBuffer(laserBufferDetails, laserWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	laser->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	laser->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	laser->SetFilterParameters(&filterParameters);
	

	laser->Start();
}

void Sound::playCrash(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* crash = getSFXVoice();

	crash->FlushSourceBuffers();
	crash->SubmitSourceBuffer(crashBufferDetails, crashWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit, X3DAUDIO_CALCULATE_MATRIX, &dspSettings);
	
	crash->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	

	crash->Start();
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

void Sound::playBoost(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* boost = getSFXVoice();

	boost->FlushSourceBuffers();
	boost->SubmitSourceBuffer(boostBufferDetails, boostWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	boost->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	boost->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	boost->SetFilterParameters(&filterParameters);
	
	boost->Start();
}

void Sound::playDropMine(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* dropmine = getSFXVoice();

	dropmine->FlushSourceBuffers();
	dropmine->SubmitSourceBuffer(dropmineBufferDetails, dropmineWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	dropmine->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	dropmine->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	dropmine->SetFilterParameters(&filterParameters);
	
	dropmine->Start();
}

void Sound::playScream(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* scream = getSFXVoice();

	scream->FlushSourceBuffers();

	// Now pick one of the three screams randomly
	int choice = std::rand() % 3;

	switch (choice) {
		case 0:	scream->SubmitSourceBuffer(scream1BufferDetails, scream1WMABuffer);
			break;
		case 1:	scream->SubmitSourceBuffer(scream2BufferDetails, scream2WMABuffer);
			break;
		case 2:	scream->SubmitSourceBuffer(scream3BufferDetails, scream3WMABuffer);
			break;
		default:
			scream->SubmitSourceBuffer(scream1BufferDetails, scream1WMABuffer);
	}

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	scream->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	scream->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	scream->SetFilterParameters(&filterParameters);
	
	scream->Start();
}

void Sound::playCarExplode(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(carexplodeBufferDetails, carexplodeWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playExplosion(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(explosionBufferDetails, explosionWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playBeep(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(beepBufferDetails, beepWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playRocketLaunch(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(rocketlaunchBufferDetails, rocketlaunchWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playPickup(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->SetVolume(2.0f);
	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(pickupBufferDetails, pickupWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playSelect(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(selectBufferDetails, selectWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playShotgun(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->SetVolume(2.5f);
	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(shotgunBufferDetails, shotgunWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playTakenLead(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(takenleadBufferDetails, takenleadWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playLostLead(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(lostleadBufferDetails, lostleadWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playNoAmmo(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(noammoBufferDetails, noammoWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playOne(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(oneBufferDetails, oneWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playTwo(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(twoBufferDetails, twoWMABuffer);

	X3DAudioCalculate(audio3DHandle, &listener, emit,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
		&dspSettings);
	
	voice->SetOutputMatrix(smSFX, 1, details.OutputFormat.Format.nChannels, dspSettings.pMatrixCoefficients);
	voice->SetFrequencyRatio(dspSettings.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f };
	voice->SetFilterParameters(&filterParameters);
	
	voice->Start();
}

void Sound::playThree(X3DAUDIO_EMITTER* emit)
{
	IXAudio2SourceVoice* voice = getSFXVoice();

	voice->FlushSourceBuffers();

	voice->SubmitSourceBuffer(threeBufferDetails, threeWMABuffer);

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