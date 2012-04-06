#include "AIMind.h"


AIMind::AIMind(Racer* _racer, TypeOfRacer _racerType)
{
	racer = _racer;
	racerType = _racerType;
	newTime = NULL;
	oldTime = time(NULL);
	lastPosition = racer->body->getPosition();
	currentWaypoint = 0;
	currentLap = 1;
	checkPointTimer = new CheckpointTimer(racer);
	speedBoost = new Ability(SPEED);
	racerHUD = NULL;
}

AIMind::~AIMind(void)
{
}

void AIMind::update(HUD* hud, Intention intention, float seconds, Waypoint* waypoints[], Waypoint* checkpoints[]){

	checkPointTime = checkPointTimer->update(checkpoints);

	updateWaypointsAndLap(seconds, waypoints);

	switch(racerType){
		case PLAYER:
			{
				// Update camera
				if (!intention.lbumpPressed)
				{
					if ((intention.cameraX != 0) || (intention.cameraY != 0))
					{
						hkReal angle;
						float height;

						angle = intention.cameraX * 0.05f;
						height = intention.cameraY * 0.05f + racer->lookHeight;

						if (height > 0.5f)
							height = 0.5f;
						else if (height < -0.5f)
							height = -0.5f;

						racer->lookHeight = height;

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
						finalLookDir.setTransformedPos(transRot, racer->lookDir);

						finalLookDir(1) = height;

						racer->lookDir.set(finalLookDir(0), finalLookDir(1), finalLookDir(2));
					}
				}

				// Update Heads Up Display
				hud->update(intention);
	

				if(hud->getSelectedAbility() == SPEED && intention.rightTrig && !speedBoost->onCooldown()){
					speedBoost->startCooldownTimer();
				}

				if(hud->getSelectedAbility() == LASER && intention.rightTrig){
					racer->fireLaser();
				}

				if(speedBoost->onCooldown()){
					char buf1[33];
					speedBoost->updateCooldown();
					std::string stringArray[] = { buf1 };//, buf2, buf3, buf4 };
					//renderer->setText(stringArray, 1);
				}


				/************* STEERING CALCULATIONS *************/
				hkVector4 A = racer->drawable->getZhkVector();
				hkVector4 C = racer->body->getPosition();
				hkVector4 B = hkVector4(racer->lookDir(0), 0, racer->lookDir(2));

				float angle = acos(B.dot3(A));

				// Sign determines if it is pointing to the right or the left of the current waypoint
				float sign = B.dot3(racer->drawable->getXhkVector());

				if (racer->currentAcceleration < 0.0f)
					sign *= -1.0f;

				// The computer only turns if it is pointing away from the current waypoint by more than 20
				// degrees in either direction.
				if ((angle > 0.0f) && (sign > 0))
				{
					racer->steer(seconds, min(angle / 1.11f, 1.0f));
				}
				else if ((angle > 0.0f) && (sign < 0))
				{
					racer->steer(seconds, -min(angle / 1.11f, 1.0f));
				}
				else
				{
					racer->steer(seconds, 0.0f);
				}

				/****************************************************/

				racer->accelerate(seconds, intention.acceleration + speedBoost->getBoostValue());

				racer->applyForces(seconds);

				break;
			}
		case COMPUTER:
			{
				/*
					The computer travels at a base speed unless affected by particular modifiers.
					These modifiers can include things like speed boost, and waypoint types.
					Each waypoint can be a different type, if it is a standard waypoint, the vehicle will not
					change speed.  If it is a turn point or sharp point though, it will travel at slower speeds
					to make turns more successfully.
				*/
				float baseSpeed = 0.8f;
				hkVector4 vel = racer->body->getLinearVelocity();
				float velocity = vel.dot3(racer->drawable->getZhkVector());
				if(currentWaypoint+1 != 80){
					if(waypoints[currentWaypoint+1]->getWaypointType() == TURN_POINT && velocity > 70.0f){
						baseSpeed = 0.0f;
					}
					if(waypoints[currentWaypoint+1]->getWaypointType() == SHARP_POINT && velocity > 45.0f){
						baseSpeed = 0.0f;
					}
				}

				/* 
					Currently, the AI only uses a speedboost if it is at waypoint 35 (the middle of the
					ramp on the racetrack), and is traveling too slow to make the jump.
				*/
				if(!speedBoost->onCooldown() && currentWaypoint == 35 && velocity < 40){
					speedBoost->startCooldownTimer();
				}

				if(speedBoost->onCooldown()){
					char buf1[33];
					speedBoost->updateCooldown();
					_itoa_s(speedBoost->getCooldownTime(), buf1, 10);
				}

				racer->accelerate(seconds, baseSpeed + speedBoost->getBoostValue());

				/************* STEERING CALCULATIONS *************/
				racer->lookDir = hkVector4(waypoints[currentWaypoint]->wpPosition);
				racer->lookDir.sub3clobberW(racer->body->getPosition());
				racer->lookDir.normalize3();

				hkVector4 A = racer->drawable->getZhkVector();
				hkVector4 C = racer->body->getPosition();
				hkVector4 B = hkVector4(racer->lookDir(0), 0, racer->lookDir(2));

				float angle = acos(B.dot3(A));

				// Sign determines if it is pointing to the right or the left of the current waypoint
				float sign = B.dot3(racer->drawable->getXhkVector());

				// The computer only turns if it is pointing away from the current waypoint by more than 20
				// degrees in either direction.
				if ((angle > 0.0f) && (sign > 0))
				{
					racer->steer(seconds, min(angle / 1.11f, 1.0f));
				}
				else if ((angle > 0.0f) && (sign < 0))
				{
					racer->steer(seconds, -min(angle / 1.11f, 1.0f));
				}
				else
				{
					racer->steer(seconds, 0.0f);
				}
				





				
				/* *********OLD CALCULATIONS ***********
				hkVector4 A = racer->drawable->getZhkVector();
				hkVector4 C = racer->body->getPosition();
				hkVector4 B = hkVector4(waypoints[currentWaypoint]->wpPosition);
				B.sub4(C);
				float Ax = A.getComponent(0); float Ay = A.getComponent(1); float Az = A.getComponent(2);
				float Bx = B.getComponent(0); float By = B.getComponent(1); float Bz = B.getComponent(2);
				float AB = Ax*Bx+Ay*By+Az*Bz;
				float Ad = sqrt(Ax*Ax + Ay*Ay + Az*Az);
				float Bd = sqrt(Bx*Bx + By*By + Bz*Bz);
				float angle = acos(AB/(Ad*Bd));

				// Sign determines if it is pointing to the right or the left of the current waypoint
				float sign = B.dot3(racer->drawable->getXhkVector());

				// The computer only turns if it is pointing away from the current waypoint by more than 20
				// degrees in either direction.
				if(angle > 0.34906 && sign > 0){
					racer->steer(seconds, 1.0f);
				}
				else if(angle > 0.34906 && sign < 0){
					racer->steer(seconds, -1.0f);
				}
				else
				{
					racer->steer(seconds, 0.0f);
				}
				**********************************/

				/****************************************************/

				racer->applyForces(seconds);

				/*
					If the computer is stuck, whether flipped, on its side,
					driving into a wall, or unable to move, if its position does
					not change for a particular amount of time, it will reset its location
					to its current waypoints location.
				*/
				hkVector4 currentPosition = racer->body->getPosition();
				int distanceTo = (int)currentPosition.distanceTo(lastPosition);
				newTime = time(NULL);
				int timeDiff = (int)difftime(newTime, oldTime);
				if(timeDiff > 1){
					if(distanceTo < 1){
						//hkVector4 racerBody = racer->body->getPosition();
						D3DXVECTOR3 cwPosition = waypoints[currentWaypoint]->drawable->getPosition();
						racer->reset(new hkVector4(cwPosition.x, cwPosition.y, cwPosition.z));
					}
					else{
						lastPosition = currentPosition;
					}
					oldTime = newTime;
				}
				
				break;
			}
		case NETWORK:
			{
				if(racerHUD == NULL)
				{
					racerHUD = new HUD(hud->width,hud->height);
				}

				// Update camera
				if (!intention.lbumpPressed)
				{
					if ((intention.cameraX != 0) || (intention.cameraY != 0))
					{
						hkReal angle;
						float height;

						angle = intention.cameraX * 0.05f;
						height = intention.cameraY * 0.05f + racer->lookHeight;

						if (height > 0.5f)
							height = 0.5f;
						else if (height < -0.5f)
							height = -0.5f;

						racer->lookHeight = height;

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
						finalLookDir.setTransformedPos(transRot, racer->lookDir);

						finalLookDir(1) = height;

						racer->lookDir.set(finalLookDir(0), finalLookDir(1), finalLookDir(2));
					}
				}

				// Update Heads Up Display
				racerHUD->update(intention);
	

				if(racerHUD->getSelectedAbility() == SPEED && intention.rightTrig && !speedBoost->onCooldown()){
					speedBoost->startCooldownTimer();
				}

				if(racerHUD->getSelectedAbility() == LASER && intention.rightTrig){
					racer->fireLaser();
				}

				if(speedBoost->onCooldown()){
					char buf1[33];
					speedBoost->updateCooldown();
					std::string stringArray[] = { buf1 };//, buf2, buf3, buf4 };
					//renderer->setText(stringArray, 1);
				}

				/************* STEERING CALCULATIONS *************/
				hkVector4 A = racer->drawable->getZhkVector();
				hkVector4 C = racer->body->getPosition();
				hkVector4 B = hkVector4(racer->lookDir(0), 0, racer->lookDir(2));

				float angle = acos(B.dot3(A));

				// Sign determines if it is pointing to the right or the left of the current waypoint
				float sign = B.dot3(racer->drawable->getXhkVector());

				if (racer->currentAcceleration < 0.0f)
					sign *= -1.0f;

				// The computer only turns if it is pointing away from the current waypoint by more than 20
				// degrees in either direction.
				if ((angle > 0.0f) && (sign > 0))
				{
					racer->steer(seconds, min(angle / 1.11f, 1.0f));
				}
				else if ((angle > 0.0f) && (sign < 0))
				{
					racer->steer(seconds, -min(angle / 1.11f, 1.0f));
				}
				else
				{
					racer->steer(seconds, 0.0f);
				}

				/****************************************************/

				racer->accelerate(seconds, intention.acceleration + speedBoost->getBoostValue());

				racer->applyForces(seconds);

				break;
			}
		case CLIENT:
			{
				hud->update(intention);
				/*
					The computer travels at a base speed unless affected by particular modifiers.
					These modifiers can include things like speed boost, and waypoint types.
					Each waypoint can be a different type, if it is a standard waypoint, the vehicle will not
					change speed.  If it is a turn point or sharp point though, it will travel at slower speeds
					to make turns more successfully.
				*/
				/*float baseSpeed = 0.8f;
				hkVector4 vel = racer->body->getLinearVelocity();
				float velocity = vel.dot3(racer->drawable->getZhkVector());
				if(currentWaypoint+1 != 80){
					if(waypoints[currentWaypoint+1]->getWaypointType() == TURN_POINT && velocity > 70.0f){
						baseSpeed = 0.0f;
					}
					if(waypoints[currentWaypoint+1]->getWaypointType() == SHARP_POINT && velocity > 45.0f){
						baseSpeed = 0.0f;
					}
				}

				racer->accelerate(seconds, baseSpeed + speedBoost->getBoostValue());*/

				/************* STEERING CALCULATIONS *************/
				/*racer->lookDir = hkVector4(waypoints[currentWaypoint]->wpPosition);
				racer->lookDir.sub3clobberW(racer->body->getPosition());
				racer->lookDir.normalize3();

				hkVector4 A = racer->drawable->getZhkVector();
				hkVector4 C = racer->body->getPosition();
				hkVector4 B = hkVector4(racer->lookDir(0), 0, racer->lookDir(2));

				float angle = acos(B.dot3(A));

				// Sign determines if it is pointing to the right or the left of the current waypoint
				float sign = B.dot3(racer->drawable->getXhkVector());

				// The computer only turns if it is pointing away from the current waypoint by more than 20
				// degrees in either direction.
				if ((angle > 0.0f) && (sign > 0))
				{
					racer->steer(seconds, min(angle / 1.11f, 1.0f));
				}
				else if ((angle > 0.0f) && (sign < 0))
				{
					racer->steer(seconds, -min(angle / 1.11f, 1.0f));
				}
				else
				{
					racer->steer(seconds, 0.0f);
				}
				
				racer->applyForces(seconds);*/

				/*
					If the computer is stuck, whether flipped, on its side,
					driving into a wall, or unable to move, if its position does
					not change for a particular amount of time, it will reset its location
					to its current waypoints location.
				*/
				/*hkVector4 currentPosition = racer->body->getPosition();
				int distanceTo = (int)currentPosition.distanceTo(lastPosition);
				newTime = time(NULL);
				int timeDiff = (int)difftime(newTime, oldTime);
				if(timeDiff > 1){
					if(distanceTo < 1){
						//hkVector4 racerBody = racer->body->getPosition();
						D3DXVECTOR3 cwPosition = waypoints[currentWaypoint]->drawable->getPosition();
						racer->reset(new hkVector4(cwPosition.x, cwPosition.y, cwPosition.z));
					}
					else{
						lastPosition = currentPosition;
					}
					oldTime = newTime;
				}*/
				
				break;
			}

	}

}

// Returns the current time left on the checkpoint timer
int AIMind::getCheckpointTime()
{
	return checkPointTime;
}

// When an AI reaches its waypoint, it will update its goal to the next waypoint
void AIMind::updateWaypointsAndLap(float seconds, Waypoint* waypoints[])
{
	if(waypoints[currentWaypoint]->withinWaypoint(&racer->body->getPosition())){
		if(waypoints[currentWaypoint]->getWaypointType() == LAP_POINT){
			currentLap += 1;
		}

		if(currentWaypoint == 79){
			currentWaypoint = 0;
		}
		else{
			currentWaypoint += 1;
		}
	}
}

// Switches between whether the racer is being controlled by a player or computer
void AIMind::togglePlayerComputerAI()
{
	if(racerType == COMPUTER){
		racerType = PLAYER;
	}
	else{
		racerType = COMPUTER;
	}
}

void AIMind::setType(TypeOfRacer type)
{
	racerType = type;
}

// Returns the current lap that the racer is on
int AIMind::getCurrentLap()
{
	return currentLap;
}

// Returns the ID of the current waypoint the racer is trying to reach
int AIMind::getCurrentWaypoint()
{
	return currentWaypoint;
}

// Returns how much time is left on the cooldown of a racer
int AIMind::getSpeedCooldown()
{
	return speedBoost->getCooldownTime();
}

TypeOfRacer AIMind::getType()
{
	return racerType;
}