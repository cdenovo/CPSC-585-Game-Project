#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Renderer.h"

#include "FrontWheel.h"


class Racer
{
public:
	Racer(IDirect3DDevice9* device, Renderer* r, Physics* p);
	~Racer(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();

	void accelerate(float seconds, float value);	// between -1.0 and 1.0 (backwards is negative)
	void turn(float seconds, float value, bool reversing);			// between -1.0 and 1.0 (left is negative)

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

	FrontWheel* wheelRL;
	FrontWheel* wheelRR;
	
	int xID, yID, zID;
	hkVector4 xAxis, yAxis, zAxis;
};

