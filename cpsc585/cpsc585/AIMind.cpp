#include "AIMind.h"


AIMind::AIMind(void)
{
	racer = NULL;
	currentWaypoint = 0;
}

AIMind::AIMind(Racer* _racer)
{
	racer = _racer;
	currentWaypoint = 0;
}

AIMind::~AIMind(void)
{
}

void AIMind::update(float seconds, Waypoint* waypoints[]){
	if(!waypoints[currentWaypoint]->withinWaypoint(&racer->body->getPosition())){
			racer->accelerate(seconds, 0.8f);
		}
		else{
			if(currentWaypoint == 3){
				currentWaypoint = 0;
			}
			else{
				currentWaypoint += 1;
			}
		}

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

	float sign = B.dot3(racer->drawable->getXhkVector());
	/*
	char buf1[33];
	_itoa_s(B.dot3(player->drawable->getXhkVector()), buf1, 10);
	std::string stringArray[] = { std::string("Angle to Current Waypoint: ").append(buf1) };
	renderer->setText(stringArray, sizeof(stringArray) / sizeof(std::string));
	*/

	if(angle > 0.34906 && sign > 0){
		racer->steer(seconds, 1.0f);
	}
	else if(angle > 0.34906 && sign < 0){
		racer->steer(seconds, -1.0f);
	}

	racer->applyForces(seconds);
	
	racer->update();
}
