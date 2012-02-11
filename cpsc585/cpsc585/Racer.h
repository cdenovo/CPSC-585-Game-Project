#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Renderer.h"

#include "FrontWheel.h"
#include "RearWheel.h"


class Racer
{
public:
	Racer(IDirect3DDevice9* device, Renderer* r, Physics* p, int racerNumber);
	~Racer(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();

	void accelerate(float seconds, float value);	// between -1.0 and 1.0 (backwards is negative)
	void steer(float seconds, float value);			// between -1.0 and 1.0 (left is negative)

	int getIndex();

private:
	void buildConstraint(hkVector4* attachmentPt, hkpGenericConstraintData* constraint);


public:
	Drawable* drawable;
	hkpRigidBody* body;

private:
	int index;
	FrontWheel* wheelFL;
	FrontWheel* wheelFR;

	RearWheel* wheelRL;
	RearWheel* wheelRR;
	
	int xID, yID, zID;
	hkVector4 xAxis, yAxis, zAxis;
	hkVector4 attachFL, attachFR, attachRL, attachRR;

	hkReal chassisMass;

	float currentSteering;
	float accelerationScale;
	float torqueScale;
	float centripScale;
};

