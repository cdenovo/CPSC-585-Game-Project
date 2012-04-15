#pragma once

#include "Drawable.h"
#include "Physics.h"
#include "Renderer.h"
#include "Sound.h"

#include "FrontWheel.h"
#include "RearWheel.h"
#include "ConfigReader.h"
#include "LaserModel.h"
#include "DynamicObjManager.h"
#include "SmokeSystem.h"

enum RacerType { RACER1, RACER2, RACER3, RACER4, RACER5, RACER6, RACER7, RACER8 };
enum WheelType { FRONT, REAR };


#define LASER_DAMAGE 50

 struct RacerData
 {
	 hkVector4 position;
	 hkQuaternion rotation;
	 hkVector4 linearVelocity;
	 hkVector4 angularVelocity;
	 hkVector4 wheelFL;
	 hkVector4 wheelFR;
	 hkVector4 wheelRL;
	 hkVector4 wheelRR;
	 hkVector4 lookDir;
	 float lookHeight;
	 bool laserCoolDown;
	 bool rocketCoolDown;
	 bool mineCoolDown;
	 bool speedCoolDown;
 };

class Racer
{
public:
	Racer(IDirect3DDevice9* device, RacerType racerType);
	~Racer(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void update();

	void brake(float seconds);
	void accelerate(float seconds, float value);	// between -1.0 and 1.0 (backwards is negative)
	void steer(float seconds, float value);			// between -1.0 and 1.0 (left is negative)

	int getIndex();
	void reset(hkVector4* resetPos, float rotation);	// Reset position and set velocity/momentum to 0
	void applyForces(float seconds);	// Call this every frame BEFORE stepping physics!
	void fireLaser();
	void fireRocket();
	void dropMine();
	void applyDamage(Racer* attacker, int damage);
	void computeRPM();

	void serialize(char buff[], bool laserOnCooldown, bool rocketOnCooldown, bool mineOnCooldown, bool speedOnCooldown);
	void unserialize(RacerData *data);

private:
	void buildConstraint(hkVector4* attachmentPt, hkpGenericConstraintData* constraint, WheelType type);
	hkVector4 getForce(hkVector4* up, hkpRigidBody* wheel, hkVector4* attach, WheelType type);
	void applySprings(float seconds);
	void applyFriction(float seconds);
	void applyDrag(float seconds);
	void applyTireRaycast();
	void respawn();
	hkpWorldRayCastInput fireWeapon();

public:
	Drawable* drawable;
	hkpRigidBody* body;
	static float accelerationScale;

	int health;
	int kills;
	int suicides;
	int deaths;
	int takenDamage;
	int givenDamage;
	float laserTime;
	
	bool laserReady;
	bool mineReady;
	bool rocketReady;
	bool speedReady;

	hkVector4 lookDir;
	float lookHeight;

	float currentAcceleration;
	bool braking;
	static ConfigReader config;

	
	X3DAUDIO_EMITTER* emitter;

	static hkVector4 attachCannon;
	static hkReal chassisMass;
	
	IXAudio2SourceVoice* engineVoice;

private:
	Drawable* laserDraw;

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

	static float rearSpringK;
	static float frontSpringK;
	static float rearDamperC;
	static float frontDamperC;
	static float rearExtents;
	static float frontExtents;
	static float springForceCap;
	static float grip;
	static float dragCoeff;
	static float topSpeed;
	static bool inverse; // Inverted look
};
