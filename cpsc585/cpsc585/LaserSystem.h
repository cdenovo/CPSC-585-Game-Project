#pragma once

#include <list>
#include "LaserBeam.h"
#include "LaserParticle.h"

#define MAX_LASER_BALL_PARTICLES 50
#define MAX_LASER_FIRE_PARTICLES 100
#define MAX_LASER_BEAM_PARTICLES 50
#define NUM_VERTICES_PER_BEAM 18

class LaserSystem
{
public:
	LaserSystem();
	~LaserSystem();
	void addLaser(hkVector4* startPoint, hkVector4* endPoint);
	void update(float seconds);
	void render();

	static LaserSystem* system;


	inline DWORD FtoDw(float f)
	{
	    return *((DWORD*)&f);
	}


private:
	std::list<LaserParticle*>* laserParticleList;
	std::list<LaserBeam*>* laserBeamList;

	IDirect3DTexture9* ballLaserTexture;
	IDirect3DTexture9* fireLaserTexture;
	IDirect3DTexture9* beamLaserTexture;

	int numBallLaser;
	int numFireLaser;
	int numBeamLaser;

	IDirect3DVertexBuffer9* ballLaserBuffer;
	IDirect3DVertexBuffer9* fireLaserBuffer;
	IDirect3DVertexBuffer9* beamLaserBuffer;
};
