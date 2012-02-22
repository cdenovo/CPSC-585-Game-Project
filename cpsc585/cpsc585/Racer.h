#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Renderer.h"

#include "FrontWheel.h"
#include "RearWheel.h"
#include "ConfigReader.h"

enum RacerType { PLAYER, AI1, AI2, AI3, AI4 };
enum WheelType { FRONT, REAR };

class Racer
{
public:
	Racer(IDirect3DDevice9* device, Renderer* r, Physics* p, RacerType racerType);
	~Racer(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();

	void accelerate(float seconds, float value);	// between -1.0 and 1.0 (backwards is negative)
	void steer(float seconds, float value);			// between -1.0 and 1.0 (left is negative)

	int getIndex();
	void reset();	// Reset position and set velocity/momentum to 0

	void applyForces(float seconds);	// Call this every frame BEFORE stepping physics!

private:
	void buildConstraint(hkVector4* attachmentPt, hkpGenericConstraintData* constraint, WheelType type);
	hkVector4 getForce(hkVector4* up, hkpRigidBody* wheel, hkVector4* attach, WheelType type);
	void applySprings(float seconds);
	void applyFriction(float seconds);
	void applyFrictionToTire(hkVector4* attachPoint, hkpRigidBody* wheelBody,
		hkVector4* xVector, hkVector4* zVector, float xFrictionForce, float zFrictionForce, float seconds);

public:
	Drawable* drawable;
	hkpRigidBody* body;
	static float accelerationScale;


private:
	int index;
	FrontWheel* wheelFL;
	FrontWheel* wheelFR;

	RearWheel* wheelRL;
	RearWheel* wheelRR;

	float currentSteering;


	// Static elements that are common between all Racers
	static int xID;
	static int yID;
	static int zID;
	
	static hkVector4 xAxis;
	static hkVector4 yAxis;
	static hkVector4 zAxis;
	static hkVector4 attachFL;
	static hkVector4 attachFR;
	static hkVector4 attachRL;
	static hkVector4 attachRR;

	static hkReal chassisMass;

	static float rearSpringK;
	static float frontSpringK;
	static float rearDamperC;
	static float frontDamperC;
	static float rearExtents;
	static float frontExtents;
	static float springForceCap;
	
	static ConfigReader config;
};

class WheelListener : public hkpContactListener
{
public:
	void collisionAddedCallback(const hkpCollisionEvent& ev);
	void collisionRemovedCallback(const hkpCollisionEvent& ev);

	WheelListener(bool* touchingGround);
	bool* touching;
};
