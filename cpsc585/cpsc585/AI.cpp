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
	world = NULL;
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
		wpEditor->closeFile();
		delete wpEditor;
		wpEditor = NULL;
	}
}

void AI::initialize(Renderer* r, Input* i, Sound* s)
{
	renderer = r;
	input = i;
	sound = s;
	count = 25;
	fps = 0;
	racerIndex = 0;

	wpEditor = new WaypointEditor(renderer);
	wpEditor->openFile();

	//Initialize physics
	physics = new Physics();
	physics->initialize(5);
	
	// Initialize sound
	s->initialize(NUMRACERS);

	//Initialize Abilities
	speedBoost = new Ability(SPEED); // Speed boost with cooldown of 15 seconds and aditional speed of 1
	
	//Initialize player
	player = new Racer(r->getDevice(), renderer, physics, sound, RACER1);
	player->setPosAndRot(-20.0f, -14.0f, -190.0f, 0.0f, 0.0f, 0.0f);
	playerMind = new AIMind(player, PLAYER);
	racers[0] = player;
	racerMinds[0] = playerMind;
	

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
	initializeWaypoints();

	//Initialize Checkpoints & Finish Lines
	initializeCheckpoints();

	//Initialize HUD
	hud = renderer->getHUD();
	checkPointTimer = new CheckpointTimer(player);

	// This is how you set an object for the camera to focus on!
	renderer->setFocus(racers[racerIndex]->getIndex());

}

void AI::initializeAIRacers()
{
	ai1 = new Racer(renderer->getDevice(), renderer, physics, sound, RACER2);
	ai1->setPosAndRot(-10.0f, -14.0f, -190.0f, 0.0f, 0.0f, 0.0f);
	aiMind1 = new AIMind(ai1, COMPUTER);
	
	ai2 = new Racer(renderer->getDevice(), renderer, physics, sound, RACER3);
	ai2->setPosAndRot(-25.0f, -14.0f, -190.0f, 0.0f, 0.0f, 0.0f);
	aiMind2 = new AIMind(ai2, COMPUTER);

	ai3 = new Racer(renderer->getDevice(), renderer, physics, sound, RACER4);
	ai3->setPosAndRot(-15.0f, -14.0f, -190.0f, 0.0f, 0.0f, 0.0f);
	aiMind3 = new AIMind(ai3, COMPUTER);

	ai4 = new Racer(renderer->getDevice(), renderer, physics, sound, RACER5);
	ai4->setPosAndRot(-5.0f, -14.0f, -190.0f, 0.0f, 0.0f, 0.0f);
	aiMind4 = new AIMind(ai4, COMPUTER);

	racers[1] = ai1;
	racers[2] = ai2;
	racers[3] = ai3;
	racers[4] = ai4;
	racerMinds[1] = aiMind1;
	racerMinds[2] = aiMind2;
	racerMinds[3] = aiMind3;
	racerMinds[4] = aiMind4;
}

void AI::initializeWaypoints()
{
	wp1 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp1->drawable);
	wp1->setPosAndRot(-4.0f, -14.0f, -157.0f, 0.0f, 0.0f, 0.0f);
	wp2 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp2->drawable);
	wp2->setPosAndRot(8.0f, -14.0f, -127.0f, 0.0f, 0.0f, 0.0f);
	wp3 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp3->drawable);
	wp3->setPosAndRot(38.0f, -14.0f, -77.0f, 0.0f, 0.0f, 0.0f);
	wp4 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp4->drawable);
	wp4->setPosAndRot(67.0f, -14.0f, -18.0f, 0.0f, 0.0f, 0.0f);
	wp5 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp5->drawable);
	wp5->setPosAndRot(70.0f, -14.0f, 31.0f, 0.0f, 0.0f, 0.0f);
	wp6 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp6->drawable);
	wp6->setPosAndRot(39.0f, -14.0f, 68.0f, 0.0f, 0.0f, 0.0f);
	wp7 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp7->drawable);
	wp7->setPosAndRot(-13.0f, -14.0f, 110.0f, 0.0f, 0.0f, 0.0f);
	wp8 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp8->drawable);
	wp8->setPosAndRot(-79.0f, -14.0f, 165.0f, 0.0f, 0.0f, 0.0f);
	wp9 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp9->drawable);
	wp9->setPosAndRot(-114.0f, -14.0f, 224.0f, 0.0f, 0.0f, 0.0f);
	wp10 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp10->drawable);
	wp10->setPosAndRot(-150.0f, -14.0f, 285.0f, 0.0f, 0.0f, 0.0f);
	wp11 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp11->drawable);
	wp11->setPosAndRot(-193.0f, -14.0f, 326.0f, 0.0f, 0.0f, 0.0f);
	wp12 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp12->drawable);
	wp12->setPosAndRot(-248.0f, -14.0f, 349.0f, 0.0f, 0.0f, 0.0f);
	wp13 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp13->drawable);
	wp13->setPosAndRot(-284.0f, -14.0f, 349.0f, 0.0f, 0.0f, 0.0f);
	wp14 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp14->drawable);
	wp14->setPosAndRot(-309.0f, -14.0f, 327.0f, 0.0f, 0.0f, 0.0f);
	wp15 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp15->drawable);
	wp15->setPosAndRot(-314.0f, -14.0f, 302.0f, 0.0f, 0.0f, 0.0f);
	wp16 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp16->drawable);
	wp16->setPosAndRot(-315.0f, -14.0f, 259.0f, 0.0f, 0.0f, 0.0f);
	wp17 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp17->drawable);
	wp17->setPosAndRot(-308.0f, -14.0f, 196.0f, 0.0f, 0.0f, 0.0f);
	wp18 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp18->drawable);
	wp18->setPosAndRot(-288.0f, -14.0f, 142.0f, 0.0f, 0.0f, 0.0f);
	wp19 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp19->drawable);
	wp19->setPosAndRot(-264.0f, -14.0f, 90.0f, 0.0f, 0.0f, 0.0f);
	wp20 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp20->drawable);
	wp20->setPosAndRot(-252.0f, -14.0f, 36.0f, 0.0f, 0.0f, 0.0f);
	wp21 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp21->drawable);
	wp21->setPosAndRot(-263.0f, -14.0f, -13.0f, 0.0f, 0.0f, 0.0f);
	wp22 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp22->drawable);
	wp22->setPosAndRot(-282.0f, -14.0f, -69.0f, 0.0f, 0.0f, 0.0f);
	wp23 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp23->drawable);
	wp23->setPosAndRot(-305.0f, -14.0f, -137.0f, 0.0f, 0.0f, 0.0f);
	wp24 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp24->drawable);
	wp24->setPosAndRot(-325.0f, -14.0f, -215.0f, 0.0f, 0.0f, 0.0f);
	wp25 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp25->drawable);
	wp25->setPosAndRot(-324.0f, -14.0f, -300.0f, 0.0f, 0.0f, 0.0f);
	wp26 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp26->drawable);
	wp26->setPosAndRot(-307.0f, -14.0f, -334.0f, 0.0f, 0.0f, 0.0f);
	wp27 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp27->drawable);
	wp27->setPosAndRot(-283.0f, -14.0f, -343.0f, 0.0f, 0.0f, 0.0f);
	wp28 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp28->drawable);
	wp28->setPosAndRot(-241.0f, -14.0f, -349.0f, 0.0f, 0.0f, 0.0f);
	wp29 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp29->drawable);
	wp29->setPosAndRot(-199.0f, -14.0f, -328.0f, 0.0f, 0.0f, 0.0f);
	wp30 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp30->drawable);
	wp30->setPosAndRot(-188.0f, -14.0f, -276.0f, 0.0f, 0.0f, 0.0f);
	wp31 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp31->drawable);
	wp31->setPosAndRot(-182.0f, -14.0f, -218.0f, 0.0f, 0.0f, 0.0f);
	wp32 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp32->drawable);
	wp32->setPosAndRot(-160.0f, -14.0f, -153.0f, 0.0f, 0.0f, 0.0f);
	wp33 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp33->drawable);
	wp33->setPosAndRot(-135.0f, -12.0f, -108.0f, 0.0f, 0.0f, 0.0f);
	wp34 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp34->drawable);
	wp34->setPosAndRot(-114.0f, -7.0f, -77.0f, 0.0f, 0.0f, 0.0f);
	wp35 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp35->drawable);
	wp35->setPosAndRot(-83.0f, -2.0f, -31.0f, 0.0f, 0.0f, 0.0f);
	wp36 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp36->drawable);
	wp36->setPosAndRot(-61.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	wp37 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp37->drawable);
	wp37->setPosAndRot(-30.0f, 7.0f, 49.0f, 0.0f, 0.0f, 0.0f);
	wp38 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp38->drawable);
	wp38->setPosAndRot(-5.0f, 10.0f, 85.0f, 0.0f, 0.0f, 0.0f);
	wp39 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp39->drawable);
	wp39->setPosAndRot(64.0f, -5.0f, 176.0f, 0.0f, 0.0f, 0.0f);
	wp40 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp40->drawable);
	wp40->setPosAndRot(94.0f, -6.0f, 213.0f, 0.0f, 0.0f, 0.0f);
	wp41 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp41->drawable);
	wp41->setPosAndRot(136.0f, -8.0f, 260.0f, 0.0f, 0.0f, 0.0f);
	wp42 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp42->drawable);
	wp42->setPosAndRot(194.0f, -14.0f, 312.0f, 0.0f, 0.0f, 0.0f);
	wp43 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp43->drawable);
	wp43->setPosAndRot(240.0f, -14.0f, 319.0f, 0.0f, 0.0f, 0.0f);
	wp44 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp44->drawable);
	wp44->setPosAndRot(278.0f, -14.0f, 312.0f, 0.0f, 0.0f, 0.0f);
	wp45 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp45->drawable);
	wp45->setPosAndRot(316.0f, -14.0f, 284.0f, 0.0f, 0.0f, 0.0f);
	wp46 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp46->drawable);
	wp46->setPosAndRot(327.0f, -14.0f, 232.0f, 0.0f, 0.0f, 0.0f);
	wp47 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp47->drawable);
	wp47->setPosAndRot(303.0f, -14.0f, 177.0f, 0.0f, 0.0f, 0.0f);
	wp48 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp48->drawable);
	wp48->setPosAndRot(270.0f, -14.0f, 144.0f, 0.0f, 0.0f, 0.0f);
	wp49 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp49->drawable);
	wp49->setPosAndRot(227.0f, -14.0f, 98.0f, 0.0f, 0.0f, 0.0f);
	wp50 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp50->drawable);
	wp50->setPosAndRot(202.0f, -14.0f, 50.0f, 0.0f, 0.0f, 0.0f);
	wp51 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp51->drawable);
	wp51->setPosAndRot(204.0f, -14.0f, 7.0f, 0.0f, 0.0f, 0.0f);
	wp52 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp52->drawable);
	wp52->setPosAndRot(223.0f, -14.0f, -21.0f, 0.0f, 0.0f, 0.0f);
	wp53 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp53->drawable);
	wp53->setPosAndRot(258.0f, -14.0f, -50.0f, 0.0f, 0.0f, 0.0f);
	wp54 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp54->drawable);
	wp54->setPosAndRot(302.0f, -14.0f, -86.0f, 0.0f, 0.0f, 0.0f);
	wp55 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp55->drawable);
	wp55->setPosAndRot(315.0f, -14.0f, -135.0f, 0.0f, 0.0f, 0.0f);
	wp56 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp56->drawable);
	wp56->setPosAndRot(293.0f, -14.0f, -165.0f, 0.0f, 0.0f, 0.0f);
	wp57 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp57->drawable);
	wp57->setPosAndRot(239.0f, -14.0f, -178.0f, 0.0f, 0.0f, 0.0f);
	wp58 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp58->drawable);
	wp58->setPosAndRot(190.0f, -14.0f, -181.0f, 0.0f, 0.0f, 0.0f);
	wp59 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp59->drawable);
	wp59->setPosAndRot(140.0f, -14.0f, -189.0f, 0.0f, 0.0f, 0.0f);
	wp60 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp60->drawable);
	wp60->setPosAndRot(91.0f, -14.0f, -206.0f, 0.0f, 0.0f, 0.0f);
	wp61 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp61->drawable);
	wp61->setPosAndRot(80.0f, -14.0f, -227.0f, 0.0f, 0.0f, 0.0f);
	wp62 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp62->drawable);
	wp62->setPosAndRot(96.0f, -14.0f, -248.0f, 0.0f, 0.0f, 0.0f);
	wp63 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp63->drawable);
	wp63->setPosAndRot(124.0f, -14.0f, -260.0f, 0.0f, 0.0f, 0.0f);
	wp64 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp64->drawable);
	wp64->setPosAndRot(167.0f, -14.0f, -269.0f, 0.0f, 0.0f, 0.0f);
	wp65 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp65->drawable);
	wp65->setPosAndRot(231.0f, -14.0f, -276.0f, 0.0f, 0.0f, 0.0f);
	wp66 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp66->drawable);
	wp66->setPosAndRot(283.0f, -14.0f, -282.0f, 0.0f, 0.0f, 0.0f);
	wp67 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp67->drawable);
	wp67->setPosAndRot(311.0f, -14.0f, -295.0f, 0.0f, 0.0f, 0.0f);
	wp68 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp68->drawable);
	wp68->setPosAndRot(323.0f, -14.0f, -314.0f, 0.0f, 0.0f, 0.0f);
	wp69 = new Waypoint(renderer->getDevice(), SHARP_POINT);
	//renderer->addDrawable(wp69->drawable);
	wp69->setPosAndRot(316.0f, -14.0f, -343.0f, 0.0f, 0.0f, 0.0f);
	wp70 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp70->drawable);
	wp70->setPosAndRot(294.0f, -14.0f, -357.0f, 0.0f, 0.0f, 0.0f);
	wp71 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp71->drawable);
	wp71->setPosAndRot(254.0f, -14.0f, -363.0f, 0.0f, 0.0f, 0.0f);
	wp72 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp72->drawable);
	wp72->setPosAndRot(206.0f, -14.0f, -360.0f, 0.0f, 0.0f, 0.0f);
	wp73 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp73->drawable);
	wp73->setPosAndRot(153.0f, -14.0f, -355.0f, 0.0f, 0.0f, 0.0f);
	wp74 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp74->drawable);
	wp74->setPosAndRot(91.0f, -14.0f, -349.0f, 0.0f, 0.0f, 0.0f);
	wp75 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp75->drawable);
	wp75->setPosAndRot(42.0f, -14.0f, -340.0f, 0.0f, 0.0f, 0.0f);
	wp76 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp76->drawable);
	wp76->setPosAndRot(7.0f, -14.0f, -326.0f, 0.0f, 0.0f, 0.0f);
	wp77 = new Waypoint(renderer->getDevice(), TURN_POINT);
	//renderer->addDrawable(wp77->drawable);
	wp77->setPosAndRot(-17.0f, -14.0f, -290.0f, 0.0f, 0.0f, 0.0f);
	wp78 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp78->drawable);
	wp78->setPosAndRot(-18.0f, -14.0f, -254.0f, 0.0f, 0.0f, 0.0f);
	wp79 = new Waypoint(renderer->getDevice(), CHECK_POINT);
	//renderer->addDrawable(wp79->drawable);
	wp79->setPosAndRot(-14.0f, -14.0f, -212.0f, 0.0f, 0.0f, 0.0f);
	wp80 = new Waypoint(renderer->getDevice(), LAP_POINT);
	//renderer->addDrawable(wp80->drawable);
	wp80->setPosAndRot(2.0f, -14.0f, -141.0f, 0.0f, 0.0f, 0.0f);
	waypoints[0] = wp1;
	waypoints[1] = wp2;
	waypoints[2] = wp3;
	waypoints[3] = wp4;
	waypoints[4] = wp5;
	waypoints[5] = wp6;
	waypoints[6] = wp7;
	waypoints[7] = wp8;
	waypoints[8] = wp9;
	waypoints[9] = wp10;
	waypoints[10] = wp11;
	waypoints[11] = wp12;
	waypoints[12] = wp13;
	waypoints[13] = wp14;
	waypoints[14] = wp15;
	waypoints[15] = wp16;
	waypoints[16] = wp17;
	waypoints[17] = wp18;
	waypoints[18] = wp19;
	waypoints[19] = wp20;
	waypoints[20] = wp21;
	waypoints[21] = wp22;
	waypoints[22] = wp23;
	waypoints[23] = wp24;
	waypoints[24] = wp25;
	waypoints[25] = wp26;
	waypoints[26] = wp27;
	waypoints[27] = wp28;
	waypoints[28] = wp29;
	waypoints[29] = wp30;
	waypoints[30] = wp31;
	waypoints[31] = wp32;
	waypoints[32] = wp33;
	waypoints[33] = wp34;
	waypoints[34] = wp35;
	waypoints[35] = wp36;
	waypoints[36] = wp37;
	waypoints[37] = wp38;
	waypoints[38] = wp39;
	waypoints[39] = wp40;
	waypoints[40] = wp41;
	waypoints[41] = wp42;
	waypoints[42] = wp43;
	waypoints[43] = wp44;
	waypoints[44] = wp45;
	waypoints[45] = wp46;
	waypoints[46] = wp47;
	waypoints[47] = wp48;
	waypoints[48] = wp49;
	waypoints[49] = wp50;
	waypoints[50] = wp51;
	waypoints[51] = wp52;
	waypoints[52] = wp53;
	waypoints[53] = wp54;
	waypoints[54] = wp55;
	waypoints[55] = wp56;
	waypoints[56] = wp57;
	waypoints[57] = wp58;
	waypoints[58] = wp59;
	waypoints[59] = wp60;
	waypoints[60] = wp61;
	waypoints[61] = wp62;
	waypoints[62] = wp63;
	waypoints[63] = wp64;
	waypoints[64] = wp65;
	waypoints[65] = wp66;
	waypoints[66] = wp67;
	waypoints[67] = wp68;
	waypoints[68] = wp69;
	waypoints[69] = wp70;
	waypoints[70] = wp71;
	waypoints[71] = wp72;
	waypoints[72] = wp73;
	waypoints[73] = wp74;
	waypoints[74] = wp75;
	waypoints[75] = wp76;
	waypoints[76] = wp77;
	waypoints[77] = wp78;
	waypoints[78] = wp79;
	waypoints[79] = wp80;
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
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	}
	// ---------------------------------------------------------------
	





	// ---------- UPDATE SOUND WITH CURRENT CAMERA POSITION/ORIENTATION --------------- //

	X3DAUDIO_LISTENER* listener = &(sound->listener);
	hkVector4 vec = hkVector4(racers[racerIndex]->lookDir);
	vec(1) = 0.0f;
	vec.normalize3();
	listener->OrientFront.x = vec(0);
	listener->OrientFront.y = vec(1);
	listener->OrientFront.z = vec(2);
	
	vec = hkVector4(racers[racerIndex]->body->getPosition());

	listener->Position.x = vec(0);
	listener->Position.y = vec(1);
	listener->Position.z = vec(2);

	vec = hkVector4(racers[racerIndex]->body->getLinearVelocity());

	listener->Velocity.x = vec(0);
	listener->Velocity.y = vec(1);
	listener->Velocity.z = vec(2);


	// -------------------------------------------------------------------------------- //





	// To manipulate a Racer, you should use the methods Racer::accelerate(float) and Racer::steer(float)
	// Both inputs should be between -1.0 and 1.0. negative means backward or left, positive is forward or right.

	// Update Checkpoint Timer
	//checkPointTimer->update(checkpoints);
	

	for(int i = 0; i < 5; i++){
		racerMinds[i]->update(hud, intention, seconds, waypoints, checkpoints, prevCheckpoints, racers);
	}
	
	updateRacerPlacement(0, 4);

	for(int i = 0; i < 5; i++){
		racerPlacement[i]->setPlacement(5-i);
	}

	if(input->placingWaypoint()){
		wpEditor->update(racers[racerIndex]);
		input->setPlaceWaypointFalse();
	}

	for(int i = 0; i < 4; i++){ // currently doesn't do anything
		waypoints[i]->update();
	}

	if(intention.bPressed){ // Changes control of Computer racer to Player, and Player racer to computer, for the currently viewed racer
		racerMinds[racerIndex]->togglePlayerComputerAI();
	}

	// Switch focus (A for player, X for AI)
	if (intention.startPressed){
		if(racerIndex == 4){
			racerIndex = 0;
		}
		else{
			racerIndex += 1;
		}
		renderer->setFocus(racers[racerIndex]->getIndex());
	}
	else if (intention.selectPressed){
		if(racerIndex == 0){
			racerIndex = 4;
		}
		else{
			racerIndex -= 1;
		}
		renderer->setFocus(racers[racerIndex]->getIndex());
	}

	hkVector4 look = racers[racerIndex]->lookDir;
	(renderer->getCamera())->setLookDir(look(0), look(1), look(2));

	// Reset the player (in case you fall over)
	if (intention.yPressed)
	{
		D3DXVECTOR3 cwPosition = waypoints[racerMinds[racerIndex]->getCurrentWaypoint()]->drawable->getPosition();
		float rotation = 0;
		racers[racerIndex]->reset(new hkVector4(cwPosition.x, cwPosition.y, cwPosition.z), rotation);
	}

	physics->step(seconds);

	for(int i = 0; i < 5; i++){
		racers[i]->update();
	}

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
		_itoa_s(racerMinds[racerIndex]->getLaserDamage(), buf18, 10);
		char buf19[33];
		_itoa_s(racerMinds[racerIndex]->getPlacement(), buf19, 10);
		char buf20[33];
		_itoa_s(racerMinds[racerIndex]->getOverallPosition(), buf20, 10);
		char buf21[33];
		_itoa_s(racerMinds[racerIndex]->getSpeedLevel(), buf21, 10);
		char buf22[33];
		_itoa_s(racerMinds[racerIndex]->getCurrentCheckpoint(), buf22, 10);
		char buf23[33];
		_itoa_s((int)(racerMinds[racerIndex]->getRotationAngle()*1000.0f), buf23, 10);
		
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
			std::string("Current Waypoint: ").append(buf11),
			std::string("Current Checkpoint: ").append(buf22),
			std::string("Checkpoint Time: ").append(buf12),
			std::string("Speed level: ").append(buf21),
			std::string("Speed Boost Cooldown: ").append(buf13),
			std::string("Health: ").append(buf16),
			std::string("Laser Level: ").append(buf17),
			std::string("Laser Damage: ").append(buf18),
			std::string("Placement: ").append(buf19),
			std::string("Overall position value: ").append(buf20),
			std::string("Rotation Angle: ").append(buf23)};
	
		renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
}
