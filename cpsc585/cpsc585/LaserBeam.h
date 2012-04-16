#pragma once

#include "Mesh.h"



class LaserBeam
{
public:
	LaserBeam(D3DXVECTOR3 startPoint, D3DXVECTOR3 endPoint);
	~LaserBeam();
	void writeVertices(Vertex* vertexList);
	void update(float seconds);

	bool destroyed;
	static float vanishingSpeed;

private:
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
	
	static float halfThickness;
	
	float beamLength;
	float timeUntilFinished;
	float currentTime;
};

