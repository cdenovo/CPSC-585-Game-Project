#include "AI.h"


AI::AI(void)
{
	renderer = NULL;
	input = NULL;
	physics = NULL;
	sound = NULL;
	hud = NULL;
	checkPointTimer = NULL;
	wpEditor = NULL;

	player = NULL;
	ai1 = NULL;
	ai2 = NULL;
	ai3 = NULL;
	ai4 = NULL;
	ai5 = NULL;
	ai6 = NULL;
	ai7 = NULL;
	world = NULL;

	dynManager = NULL;
}


AI::~AI(void)
{
}

void AI::shutdown()
{
	if (player)
	{
		delete player;
		player = NULL;
	}
	
	if (ai1)
	{
		delete ai1;
		ai1 = NULL;
	}
	
	if(ai2)
	{
		delete ai2;
		ai2 = NULL;
	}

	if(ai3)
	{
		delete ai3;
		ai3 = NULL;
	}

	if(ai4)
	{
		delete ai4;
		ai4 = NULL;
	}

	if(ai5)
	{
		delete ai5;
		ai5 = NULL;
	}

	if(ai6)
	{
		delete ai6;
		ai6 = NULL;
	}

	if(ai7)
	{
		delete ai7;
		ai7 = NULL;
	}
	
	if (world)
	{
		delete world;
		world = NULL;
	}

	if (physics)
	{
		physics->shutdown();
		delete physics;
		physics = NULL;
	}

	if(checkPointTimer)
	{
		delete checkPointTimer;
		checkPointTimer = NULL;
	}

	if (wpEditor)
	{
		//wpEditor->closeFile();
		delete wpEditor;
		wpEditor = NULL;
	}

	if (dynManager)
	{
		delete dynManager;
		dynManager = NULL;
	}
}

void AI::initialize(Renderer* r, Input* i, Sound* s)
{
	raceStartTimer = 4.0f;
	raceStarted = false;
	raceEnded = false;
	playedOne = false;
	playedTwo = false;
	playedThree = false;
	generatePostGameStatistics = true;

	renderer = r;
	input = i;
	sound = s;
	count = 25;
	fps = 0;
	racerIndex = 0;
	numberOfWaypoints = 0;

	dynManager = new DynamicObjManager();

	wpEditor = new WaypointEditor(renderer);
	//wpEditor->openFile();

	//Initialize physics
	physics = new Physics();
	physics->initialize(NUMRACERS + 1);
	
	// Initialize sound
	s->initialize();

	//Initialize Abilities
	speedBoost = new Ability(SPEED); // Speed boost with cooldown of 15 seconds and aditional speed of 1
	
	//Initialize player
	player = new Racer(r->getDevice(), RACER1);
	player->setPosAndRot(35.0f, 15.0f, -298.0f, 0.0f, 1.4f, 0.0f);
	playerMind = new AIMind(player, PLAYER, NUMRACERS, "Herald", "Red");
	racers[0] = player;
	racerMinds[0] = playerMind;
	sound->playerEmitter = player->emitter;
	

	//Initialize world
	world = new World(r->getDevice(), renderer, physics);
	world->setPosAndRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	//Initialize AI-Racers
	initializeAIRacers();

	//Initialize Racer Placement
	for(int i = 0; i < NUMRACERS; i++){
		racerPlacement[i] = racerMinds[i];
	}

	//Initialize Waypoints
	wpEditor->loadWaypoints(waypoints, "RaceTrack.txt"); 
	buildingWaypoint = new Waypoint(renderer->getDevice(), WAY_POINT);
	buildingWaypoint->setPosAndRot(258.0f, 31.0f, 85.0f, 0.0f, 0.0f, 0.0f);

	
	// Initializing racer's look direction at game start.
	D3DXVECTOR3 target = waypoints[0]->drawable->getPosition();
	hkVector4 targetPos = hkVector4(target.x, target.y, target.z);
	hkVector4 shooterPos = player->body->getPosition();
	shooterPos(1) += 2.0f;
	targetPos.sub(shooterPos);
	targetPos.normalize3();

	player->lookDir.setXYZ(targetPos);
	
	//Initialize Checkpoints & Finish Lines
	//initializeCheckpoints();

	//Initialize HUD
	hud = renderer->getHUD();
	//checkPointTimer = new CheckpointTimer(player);

	// This is how you set an object for the camera to focus on!
	renderer->setFocus(racers[racerIndex]->getIndex());
}

void AI::initializeAIRacers()
{
	ai1 = new Racer(renderer->getDevice(), RACER2);
	ai1->setPosAndRot(40.0f, 15.0f, -294.0f, 0.0f, 1.4f, 0.0f);
	aiMind1 = new AIMind(ai1, COMPUTER, NUMRACERS, "Gerard", "Blue");
	
	ai2 = new Racer(renderer->getDevice(), RACER3);
	ai2->setPosAndRot(40.0f, 15.0f, -298.0f, 0.0f, 1.4f, 0.0f);
	aiMind2 = new AIMind(ai2, COMPUTER, NUMRACERS, "Nevvel", "Orange");

	ai3 = new Racer(renderer->getDevice(), RACER4);
	ai3->setPosAndRot(40.0f, 15.0f, -302.0f, 0.0f, 1.4f, 0.0f);
	aiMind3 = new AIMind(ai3, COMPUTER, NUMRACERS, "Rosey", "Green");

	ai4 = new Racer(renderer->getDevice(), RACER5);
	ai4->setPosAndRot(40.0f, 15.0f, -306.0f, 0.0f, 1.4f, 0.0f);
	aiMind4 = new AIMind(ai4, COMPUTER, NUMRACERS, "Delilah", "Teal");

	ai5 = new Racer(renderer->getDevice(), RACER6);
	ai5->setPosAndRot(35.0f, 15.0f, -306.0f, 0.0f, 1.4f, 0.0f);
	aiMind5 = new AIMind(ai5, COMPUTER, NUMRACERS, "Gupreet", "Yellow");

	ai6 = new Racer(renderer->getDevice(), RACER7);
	ai6->setPosAndRot(35.0f, 15.0f, -302.0f, 0.0f, 1.4f, 0.0f);
	aiMind6 = new AIMind(ai6, COMPUTER, NUMRACERS, "Tiffany", "Purple");

	ai7 = new Racer(renderer->getDevice(), RACER8);
	ai7->setPosAndRot(35.0f, 15.0f, -294.0f, 0.0f, 1.4f, 0.0f);
	aiMind7 = new AIMind(ai7, COMPUTER, NUMRACERS, "Rickardo", "Pink");

	racers[1] = ai1;
	racers[2] = ai2;
	racers[3] = ai3;
	racers[4] = ai4;
	racers[5] = ai5;
	racers[6] = ai6;
	racers[7] = ai7;
	racerMinds[1] = aiMind1;
	racerMinds[2] = aiMind2;
	racerMinds[3] = aiMind3;
	racerMinds[4] = aiMind4;
	racerMinds[5] = aiMind5;
	racerMinds[6] = aiMind6;
	racerMinds[7] = aiMind7;
}


void AI::initializeCheckpoints()
{
	prevCheckpoints[0] = new Waypoint(renderer->getDevice(), TURN_POINT);
	prevCheckpoints[0]->setPosAndRot(-264.0f, -14.0f, 90.0f, 0.0f, 0.0f, 0.0f);

	checkpoints[0] = new Waypoint(renderer->getDevice(), TURN_POINT);
	checkpoints[0]->setPosAndRot(-252.0f, -14.0f, 36.0f, 0.0f, 0.0f, 0.0f);

	prevCheckpoints[1] = new Waypoint(renderer->getDevice(), CHECK_POINT);
	prevCheckpoints[1]->setPosAndRot(-30.0f, 7.0f, 49.0f, 0.0f, 0.0f, 0.0f);

	checkpoints[1] = new Waypoint(renderer->getDevice(), CHECK_POINT);
	checkpoints[1]->setPosAndRot(-5.0f, 10.0f, 85.0f, 0.0f, 0.0f, 0.0f);

	prevCheckpoints[2] = new Waypoint(renderer->getDevice(), CHECK_POINT);
	prevCheckpoints[2]->setPosAndRot(239.0f, -14.0f, -178.0f, 0.0f, 0.0f, 0.0f);

	checkpoints[2] = new Waypoint(renderer->getDevice(), CHECK_POINT);
	checkpoints[2]->setPosAndRot(190.0f, -14.0f, -181.0f, 0.0f, 0.0f, 0.0f);

	prevCheckpoints[3] = new Waypoint(renderer->getDevice(), CHECK_POINT);
	prevCheckpoints[3]->setPosAndRot(-14.0f, -14.0f, -212.0f, 0.0f, 0.0f, 0.0f);

	checkpoints[3] = new Waypoint(renderer->getDevice(), LAP_POINT);
	checkpoints[3]->setPosAndRot(2.0f, -14.0f, -141.0f, 0.0f, 0.0f, 0.0f);

	renderer->addDrawable(checkpoints[0]->drawable);
	renderer->addDrawable(checkpoints[1]->drawable);
	renderer->addDrawable(checkpoints[2]->drawable);
	renderer->addDrawable(checkpoints[3]->drawable);
}

void AI::simulate(float seconds)
{
	_ASSERT(seconds > 0.0f);

	Intention intention = input->getIntention();

	// Debugging Information ---------------------------------------
	if(input->debugging()){
		displayDebugInfo(intention, seconds);
	}
	else{
		std::string stringArray[] = {""};
		//renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	// ---------------------------------------------------------------
	
	if(playerMind->isfinishedRace())
	{
		displayPostGameStatistics();
		raceEnded = true;
	}


	// ---------- UPDATE SOUND WITH CURRENT CAMERA POSITION/ORIENTATION --------------- //

	X3DAUDIO_LISTENER* listener = &(sound->listener);
	hkVector4 vec;
	vec.setXYZ(racers[racerIndex]->lookDir);
	vec(1) = 0.0f;
	vec.normalize3();
	listener->OrientFront.x = vec(0);
	listener->OrientFront.y = vec(1);
	listener->OrientFront.z = vec(2);
	
	vec.setXYZ(racers[racerIndex]->body->getPosition());

	listener->Position.x = vec(0);
	listener->Position.y = vec(1);
	listener->Position.z = vec(2);

	vec.setXYZ(racers[racerIndex]->body->getLinearVelocity());

	listener->Velocity.x = vec(0);
	listener->Velocity.y = vec(1);
	listener->Velocity.z = vec(2);


	// -------------------------------------------------------------------------------- //


	if (!raceStarted)
	{
		raceStartTimer -= seconds;

		updateRacerPlacement(0, NUMRACERS - 1);

		for (int i = 0; i < NUMRACERS; i++)
		{
			racerPlacement[i]->setPlacement(NUMRACERS-i);
		}

		if (!intention.lbumpPressed)
			{

			// Let player move camera before race starts
			hkReal angle;
			float height;

			angle = intention.cameraX * 0.05f;

			if (racers[racerIndex]->config.inverse)
				height = intention.cameraY * -0.02f + racers[racerIndex]->lookHeight;
			else
				height = intention.cameraY * 0.02f + racers[racerIndex]->lookHeight;

			if (height > 0.5f)
				height = 0.5f;
			else if (height < -0.5f)
				height = -0.5f;

			racers[racerIndex]->lookHeight = height;

			if (angle > M_PI)
				angle = (hkReal) M_PI;
			else if (angle < -M_PI)
				angle = (hkReal) -M_PI;


			hkQuaternion rotation;

			if (angle < 0.0f)
			{
				angle *= -1;
				rotation.setAxisAngle(hkVector4(0,-1,0), angle);
			}
			else
			{
				rotation.setAxisAngle(hkVector4(0,1,0), angle);
			}

			hkTransform transRot;
			transRot.setIdentity();
			transRot.setRotation(rotation);

			hkVector4 finalLookDir(0,0,1);
			finalLookDir.setTransformedPos(transRot, racers[racerIndex]->lookDir);

			finalLookDir(1) = height;

			racers[racerIndex]->lookDir.setXYZ(finalLookDir);
		}

		// Update Heads Up Display
		hud->update(intention);

		hud->setSpeed(0.0f);
		hud->setHealth(100);
		hud->setPosition(racerPlacement[racerIndex]->getPlacement());
		hud->setLap(1, racerMinds[racerIndex]->numberOfLapsToWin);

		hkVector4 look = racers[racerIndex]->lookDir;
		(renderer->getCamera())->setLookDir(look(0), look(1), look(2));
		

		for (int i = 0; i < NUMRACERS; i++)
		{
			racers[i]->applyForces(seconds);
			racers[i]->computeRPM();
		}

		physics->step(seconds);

		for (int i = 0; i < NUMRACERS; i++)
		{
			racers[i]->update();
		}
		

		if (raceStartTimer <= 0.0f)
		{
			// Show "GO" on screen, play sound
			Sound::sound->playSoundEffect(SFX_SHOTGUN, Sound::sound->playerEmitter);

			raceStartTimer = -0.1f;
			raceStarted = true;
			hud->showOne = false;
		}
		else if (raceStartTimer <= 1.0f)
		{
			if (!playedOne)
			{
				// Play "One" sound
				Sound::sound->playSoundEffect(SFX_ONE, Sound::sound->playerEmitter);
				playedOne = true;

				// Start playing music
				Sound::sound->playInGameMusic();
			}

			// Show "1" on screen
			hud->showTwo = false;
			hud->showOne = true;

			return;
		}
		else if (raceStartTimer <= 2.0f)
		{
			if (!playedTwo)
			{
				// Play "Two" sound
				Sound::sound->playSoundEffect(SFX_TWO, Sound::sound->playerEmitter);
				playedTwo = true;
			}

			// Show "2" on screen
			hud->showThree = false;
			hud->showTwo = true;

			return;
		}
		else if (raceStartTimer <= 3.0f)
		{
			if (!playedThree)
			{
				// Play "Three" sound
				Sound::sound->playSoundEffect(SFX_THREE, Sound::sound->playerEmitter);
				playedThree = true;
			}

			// Show "3" on screen
			hud->showThree = true;

			return;
		}
		else
		{

			return;
		}
	}


	// Update Checkpoint Timer
	//checkPointTimer->update(checkpoints);
	

	for(int i = 0; i < NUMRACERS; i++){
		racerMinds[i]->update(hud, intention, seconds, waypoints, racers, racerPlacement, buildingWaypoint);
	}


	
	updateRacerPlacement(0, NUMRACERS - 1);

	for(int i = 0; i < NUMRACERS; i++){
		racerPlacement[i]->setPlacement(NUMRACERS-i);
	}

	if(input->placingWaypoint()){
		
		//wpEditor->update(racers[racerIndex]);
		numberOfWaypoints += 1;
		Waypoint* addWaypoint = new Waypoint(renderer->getDevice(), input->wpType);
		hkVector4 wpPosition = racers[0]->body->getPosition();
		addWaypoint->setPosAndRot(wpPosition.getComponent(0), wpPosition.getComponent(1), wpPosition.getComponent(2), 0, 0, 0);
		wpEditor->waypoints.push_back(addWaypoint); 
		input->setPlaceWaypointFalse();
		
	}

	for(int i = 0; i < 4; i++){ // currently doesn't do anything
		waypoints[i]->update();
	}

	if(intention.bPressed){ // Changes control of Computer racer to Player, and Player racer to computer, for the currently viewed racer
		racerMinds[racerIndex]->togglePlayerComputerAI(waypoints);
	}

	// Switch focus (A for player, X for AI)
	if (intention.startPressed){
		if(racerIndex == 7){
			racerIndex = 0;
		}
		else{
			racerIndex += 1;
		}
		renderer->setFocus(racers[racerIndex]->getIndex());
	}
	else if (intention.selectPressed){
		if(racerIndex == 0){
			racerIndex = 7;
		}
		else{
			racerIndex -= 1;
		}

		renderer->setFocus(racers[racerIndex]->getIndex());
	}

	hkVector4 look = racers[racerIndex]->lookDir;

		if(raceEnded){
			hkVector4 racerPos = racers[racerIndex]->body->getPosition();
			hkSimdReal camX = racerPos.getComponent(0);
			hkSimdReal camY = racerPos.getComponent(1);
			hkSimdReal camZ = racerPos.getComponent(2);
			camX.add(4);
			camY.add(4);
			camZ.sub(2);
			hkVector4 camPos = hkVector4(camX, camY, camZ);
			racerPos.sub(camPos);
			(renderer->getCamera())->setLookDir(racerPos(0), racerPos(1), racerPos(2));
		}
		else{
			(renderer->getCamera())->setLookDir(look(0), look(1), look(2));
		}

	// Reset the player (in case you fall over)
	if (intention.yPressed)
	{
		D3DXVECTOR3 cwPosition = waypoints[racerMinds[racerIndex]->getCurrentWaypoint()]->drawable->getPosition();
		float rotation = 0;
		racers[racerIndex]->reset(&(hkVector4(cwPosition.x, cwPosition.y, cwPosition.z)), rotation);
	}

	if(intention.xPressed){
		//wpEditor->writeToFile(wpEditor->waypoints, numberOfWaypoints, "RaceTrack.txt");
	}
	if(intention.aPressed){
		
		vector<Waypoint*> passWaypoints = vector<Waypoint*>();
		for(int i = 0; i < NUMWAYPOINTS; i++){
			passWaypoints.push_back(waypoints[i]);
		}
		//wpEditor->writeToFile(passWaypoints, NUMWAYPOINTS, "Figure8Waypoints.txt");
		
	}

	
	
	physics->step(seconds);



	for(int i = 0; i < NUMRACERS; i++){
		racers[i]->update();
	}


	DynamicObjManager::manager->update(seconds);
	SmokeSystem::system->update(seconds);
	LaserSystem::system->update(seconds);
	

	return;
}

/*
	Runs a quicksort algorithm O(nlogn) to determine the position of a racer
	in the race based on numbers calculated from the most recent waypoint a
	racer has reached and what lap they are on. "OverallPosition = #waypoints x #laps"
	Source: http://www.algolist.net/Algorithms/Sorting/Quicksort
 */
void AI::updateRacerPlacement(int left, int right)
{
	int i = left, j = right;
    AIMind* tmp;

	int pivot = racerPlacement[(left + right) / 2]->getOverallPosition();

 

      /* partition */

      while (i <= j) {

		  while (racerPlacement[i]->getOverallPosition() < pivot)
                  i++;
		  while (racerPlacement[j]->getOverallPosition() > pivot)
                  j--;

            if (i <= j) {
				if(racerPlacement[i]->getCurrentWaypoint() == racerPlacement[j]->getCurrentWaypoint()){
					
					hkSimdReal distanceOfI = waypoints[racerPlacement[i]->getCurrentWaypoint()]->wpPosition.distanceTo(racerPlacement[i]->getRacerPosition());
					hkSimdReal distanceOfJ = waypoints[racerPlacement[j]->getCurrentWaypoint()]->wpPosition.distanceTo(racerPlacement[j]->getRacerPosition());

					if(distanceOfI < distanceOfJ){
						tmp = racerPlacement[i];
					    racerPlacement[i] = racerPlacement[j];
					    racerPlacement[j] = tmp;
					}
				}
				else{
					tmp = racerPlacement[i];
					    racerPlacement[i] = racerPlacement[j];
					    racerPlacement[j] = tmp;
				}
                  
                  i++;
                  j--;
            }

      }

 

      /* recursion */

      if (left < j)
            updateRacerPlacement(left, j);

      if (i < right)
            updateRacerPlacement(i, right);

}


std::string AI::getFPSString(float milliseconds)
{
	count++;

	if (count > 20)
	{
		fps = (int) floor(1000.0f / milliseconds);
		count = 0;
	}
	
	char tmp[5];

	_itoa_s(fps, tmp, 5, 10);

	return std::string("FPS: ").append((const char*) tmp);
}

std::string AI::boolToString(bool boolean)
{
	if(boolean == true){
		return "True";
	}
	else {
		return "False";
	}
}

void AI::displayDebugInfo(Intention intention, float seconds)
{
		char buf1[33];
		_itoa_s(intention.rightTrig, buf1, 10);
		char buf2[33];
		_itoa_s(intention.leftTrig, buf2, 10);
		char buf3[33];
		_itoa_s(intention.rightStickX, buf3, 10);
		char buf4[33];
		_itoa_s(intention.rightStickY, buf4, 10);
		char buf5[33];
		_itoa_s(intention.leftStickX, buf5, 10);
		char buf6[33];
		_itoa_s((int) (intention.acceleration * 100.0f), buf6, 10);
		char buf7[33];
		_itoa_s((int) (intention.leftStickY), buf7, 10);

		char buf8[33];
		hkVector4 vel = racers[racerIndex]->body->getLinearVelocity();
		float velocity = vel.dot3(racers[racerIndex]->drawable->getZhkVector());
		_itoa_s((int) (velocity), buf8, 10);

		char buf9[33];
		_itoa_s((int) (Racer::accelerationScale), buf9, 10);

		char buf10[33];
		_itoa_s((int) (racers[racerIndex]->kills), buf10, 10);
		char buf11[33];
		_itoa_s(racerMinds[racerIndex]->getCurrentWaypoint(), buf11, 10);
		char buf12[33];
		_itoa_s(racerMinds[racerIndex]->getCheckpointTime(), buf12, 10);
		char buf13[33];
		_itoa_s(racerMinds[racerIndex]->getSpeedCooldown(), buf13, 10);
		char buf14[33];
		_itoa_s(racerIndex, buf14, 10);
		char buf15[33];
		_itoa_s(racerMinds[racerIndex]->getCurrentLap(), buf15, 10);
		char buf16[33];
		_itoa_s(racers[racerIndex]->health, buf16, 10);
		char buf17[33];
		_itoa_s(racerMinds[racerIndex]->getLaserLevel(), buf17, 10);
		char buf18[33];
		_itoa_s(racerMinds[racerIndex]->getPlacement(), buf18, 10);
		char buf19[33];
		_itoa_s(racerMinds[racerIndex]->getOverallPosition(), buf19, 10);
		char buf20[33];
		_itoa_s(racerMinds[racerIndex]->getSpeedLevel(), buf20, 10);
		char buf21[33];
		_itoa_s(racerMinds[racerIndex]->getCurrentCheckpoint(), buf21, 10);
		char buf22[33];
		_itoa_s((int)(racerMinds[racerIndex]->getRotationAngle()*1000.0f), buf22, 10);
		char buf23[33];
		_itoa_s((int)(racerMinds[racerIndex]->getSpeedAmmo()), buf23, 10);
		char buf24[33];
		_itoa_s((int)(racerMinds[racerIndex]->getRocketAmmo()), buf24, 10);
		char buf25[33];
		_itoa_s((int)(racerMinds[racerIndex]->getLandmineAmmo()), buf25, 10);
		char buf26[33];
		_itoa_s(racers[racerIndex]->suicides, buf26, 10);
		char buf27[33];
		_itoa_s(racers[racerIndex]->deaths, buf27, 10);
		char buf28[33];
		_itoa_s(racers[racerIndex]->givenDamage, buf28, 10);
		char buf29[33];
		_itoa_s(racers[racerIndex]->takenDamage, buf29, 10);
		
		std::string stringArray[] = { getFPSString(seconds * 1000.0f), 
			"X: " + boolToString(intention.xPressed),
			"Y: " + boolToString(intention.yPressed),
			"A: " + boolToString(intention.aPressed),
			"B: " + boolToString(intention.bPressed),
			"Back: " + boolToString(intention.selectPressed),
			"Start: " + boolToString(intention.startPressed),
			std::string("Right Trigger: ").append(buf1),
			std::string("Left Trigger: ").append(buf2),
			std::string("RStick X: ").append(buf3),
			std::string("RStick Y: ").append(buf4),
			std::string("LStickX: ").append(buf5),
			std::string("LStickY: ").append(buf7),
			std::string("Acceleration: ").append(buf6),
			" ",
			"Player Information:",
			std::string("Currently Looking at Player #").append(buf14),
			std::string("Velocity: ").append(buf8),
			std::string("Accel. Scale: ").append(buf9),
			std::string("Current Lap: ").append(buf15),
			std::string("Kills: ").append(buf10),
			std::string("Deaths: ").append(buf27),
			std::string("Suicides: ").append(buf26),
			std::string("Given Damage: ").append(buf28),
			std::string("Taken Damage: ").append(buf29),
			std::string("Current Waypoint: ").append(buf11),
			//std::string("Current Checkpoint: ").append(buf22),
			//std::string("Checkpoint Time: ").append(buf12),
			//std::string("Speed level: ").append(buf21),
			//std::string("Speed Boost Cooldown: ").append(buf13),
			std::string("Health: ").append(buf16),
			//std::string("Laser Level: ").append(buf17),
			std::string("Placement: ").append(buf18),
			std::string("Overall position value: ").append(buf19),
			//std::string("Rotation Angle: ").append(buf22),
			std::string("Ammo Counts:"),
			std::string("Speed Boost: ").append(buf23),
			std::string("Rocket: ").append(buf24),
			std::string("Landmine: ").append(buf25)};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
}

void AI::displayPostGameStatistics()
{
	if(generatePostGameStatistics){
	char buf1[33];
	_itoa_s((int) (racerPlacement[7]->getKills()), buf1, 10);
	char buf2[33];
	_itoa_s((int) (racerPlacement[6]->getKills()), buf2, 10);
	char buf3[33];
	_itoa_s((int) (racerPlacement[5]->getKills()), buf3, 10);
	char buf4[33];
	_itoa_s((int) (racerPlacement[4]->getKills()), buf4, 10);
	char buf5[33];
	_itoa_s((int) (racerPlacement[3]->getKills()), buf5, 10);
	char buf26[33];
	_itoa_s((int) (racerPlacement[2]->getKills()), buf26, 10);
	char buf27[33];
	_itoa_s((int) (racerPlacement[1]->getKills()), buf27, 10);
	char buf28[33];
	_itoa_s((int) (racerPlacement[0]->getKills()), buf28, 10);

	char buf6[33];
	_itoa_s((int) (racerPlacement[7]->getDeaths()), buf6, 10);
	char buf7[33];
	_itoa_s((int) (racerPlacement[6]->getDeaths()), buf7, 10);
	char buf8[33];
	_itoa_s((int) (racerPlacement[5]->getDeaths()), buf8, 10);
	char buf9[33];
	_itoa_s((int) (racerPlacement[4]->getDeaths()), buf9, 10);
	char buf10[33];
	_itoa_s((int) (racerPlacement[3]->getDeaths()), buf10, 10);
	char buf29[33];
	_itoa_s((int) (racerPlacement[2]->getDeaths()), buf29, 10);
	char buf30[33];
	_itoa_s((int) (racerPlacement[1]->getDeaths()), buf30, 10);
	char buf31[33];
	_itoa_s((int) (racerPlacement[0]->getDeaths()), buf31, 10);

	char buf11[33];
	_itoa_s((int) (racerPlacement[7]->getSuicides()), buf11, 10);
	char buf12[33];
	_itoa_s((int) (racerPlacement[6]->getSuicides()), buf12, 10);
	char buf13[33];
	_itoa_s((int) (racerPlacement[5]->getSuicides()), buf13, 10);
	char buf14[33];
	_itoa_s((int) (racerPlacement[4]->getSuicides()), buf14, 10);
	char buf15[33];
	_itoa_s((int) (racerPlacement[3]->getSuicides()), buf15, 10);
	char buf32[33];
	_itoa_s((int) (racerPlacement[2]->getSuicides()), buf32, 10);
	char buf33[33];
	_itoa_s((int) (racerPlacement[1]->getSuicides()), buf33, 10);
	char buf34[33];
	_itoa_s((int) (racerPlacement[0]->getSuicides()), buf34, 10);

	char buf16[33];
	_itoa_s((int) (racerPlacement[7]->getDamageDone()), buf16, 10);
	char buf17[33];
	_itoa_s((int) (racerPlacement[6]->getDamageDone()), buf17, 10);
	char buf18[33];
	_itoa_s((int) (racerPlacement[5]->getDamageDone()), buf18, 10);
	char buf19[33];
	_itoa_s((int) (racerPlacement[4]->getDamageDone()), buf19, 10);
	char buf20[33];
	_itoa_s((int) (racerPlacement[3]->getDamageDone()), buf20, 10);
	char buf35[33];
	_itoa_s((int) (racerPlacement[2]->getDamageDone()), buf35, 10);
	char buf36[33];
	_itoa_s((int) (racerPlacement[1]->getDamageDone()), buf36, 10);
	char buf37[33];
	_itoa_s((int) (racerPlacement[0]->getDamageDone()), buf37, 10);

	char buf21[33];
	_itoa_s((int) (racerPlacement[7]->getDamageTaken()), buf21, 10);
	char buf22[33];
	_itoa_s((int) (racerPlacement[6]->getDamageTaken()), buf22, 10);
	char buf23[33];
	_itoa_s((int) (racerPlacement[5]->getDamageTaken()), buf23, 10);
	char buf24[33];
	_itoa_s((int) (racerPlacement[4]->getDamageTaken()), buf24, 10);
	char buf25[33];
	_itoa_s((int) (racerPlacement[3]->getDamageTaken()), buf25, 10);
	char buf38[33];
	_itoa_s((int) (racerPlacement[2]->getDamageTaken()), buf38, 10);
	char buf39[33];
	_itoa_s((int) (racerPlacement[1]->getDamageTaken()), buf39, 10);
	char buf40[33];
	_itoa_s((int) (racerPlacement[0]->getDamageTaken()), buf40, 10);

	postGameStatistics[0] = std::string("     Player Name:     Colour:     Kills:     Deaths:     Suicides:     Damage Done:     Damage Taken:");
	postGameStatistics[1] = std::string("1st: " + getSpaces(racerPlacement[7]->getRacerName(),17) + getSpaces(racerPlacement[7]->getRacerColour(),12) + getSpaces(buf1,11) + getSpaces(buf6,12)  + getSpaces(buf11,14) + getSpaces(buf16,17) + buf21); // 1st place
	postGameStatistics[2] = std::string("2nd: " + getSpaces(racerPlacement[6]->getRacerName(),17) + getSpaces(racerPlacement[6]->getRacerColour(),12) + getSpaces(buf2,11) + getSpaces(buf7,12)  + getSpaces(buf12,14) + getSpaces(buf17,17) + buf22); // 2nd place
	postGameStatistics[3] = std::string("3rd: " + getSpaces(racerPlacement[5]->getRacerName(),17) + getSpaces(racerPlacement[5]->getRacerColour(),12) + getSpaces(buf3,11) + getSpaces(buf8,12)  + getSpaces(buf13,14) + getSpaces(buf18,17) + buf23); // 3rd place
	postGameStatistics[4] = std::string("4th: " + getSpaces(racerPlacement[4]->getRacerName(),17) + getSpaces(racerPlacement[4]->getRacerColour(),12) + getSpaces(buf4,11) + getSpaces(buf9,12)  + getSpaces(buf14,14) + getSpaces(buf19,17) + buf24); // 4th place
	postGameStatistics[5] = std::string("5th: " + getSpaces(racerPlacement[3]->getRacerName(),17) + getSpaces(racerPlacement[3]->getRacerColour(),12) + getSpaces(buf5,11) + getSpaces(buf10,12) + getSpaces(buf15,14) + getSpaces(buf20,17) + buf25);  // 5th place
	postGameStatistics[6] = std::string("6th: " + getSpaces(racerPlacement[2]->getRacerName(),17) + getSpaces(racerPlacement[2]->getRacerColour(),12) + getSpaces(buf26,11) + getSpaces(buf29,12) + getSpaces(buf32,14) + getSpaces(buf35,17) + buf38); // 6th place
	postGameStatistics[7] = std::string("7th: " + getSpaces(racerPlacement[1]->getRacerName(),17) + getSpaces(racerPlacement[1]->getRacerColour(),12) + getSpaces(buf27,11) + getSpaces(buf30,12) + getSpaces(buf33,14) + getSpaces(buf36,17) + buf39); // 7th place
	postGameStatistics[8] = std::string("8th: " + getSpaces(racerPlacement[0]->getRacerName(),17) + getSpaces(racerPlacement[0]->getRacerColour(),12) + getSpaces(buf28,11) + getSpaces(buf31,12) + getSpaces(buf34,14) + getSpaces(buf37,17) + buf40); // 8th place

	generatePostGameStatistics = false;
	}

	renderer->setText(postGameStatistics, sizeof(postGameStatistics) / sizeof(std::string));
}

std::string AI::getSpaces(std::string input, int numSpaces){
	std::string output = input;
	for(int i = 0; i < (int)numSpaces - (int)input.size(); i++){
		output.append(" ");
	}
	return output;
}
