#pragma once
#include "Intention.h"
#include "Drawable.h"
#include "Racer.h"
#include "Waypoint.h"

#include <time.h>

class CheckpointTimer
{
public:
	CheckpointTimer(Racer* racer);
	~CheckpointTimer(void);
	int update(Waypoint* checkpoints[], Waypoint* prevCheckpoints[]);
	int getCheckpointTime();
	int getCurrentCheckpoint();
	bool downgradeAbility();

private:
	time_t oldTime;
	time_t newTime;
	int resetTime;

	double checkPointTime;
	int currentCheckpoint;
	bool downgradeAbilityTruth;

	Racer* racer;
};

