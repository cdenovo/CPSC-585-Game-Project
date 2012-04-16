#include "LaserBeam.h"

float LaserBeam::vanishingSpeed = 100.0f;
float LaserBeam::halfThickness = 0.2f;

LaserBeam::LaserBeam(D3DXVECTOR3 s, D3DXVECTOR3 e)
{
	destroyed = false;

	start = s;
	end = e;

	D3DXVECTOR3 ray = e - s;

	beamLength = D3DXVec3Length(&ray);
	timeUntilFinished = beamLength / vanishingSpeed;
	currentTime = 0.0f;
}


LaserBeam::~LaserBeam()
{
}


void LaserBeam::writeVertices(Vertex* vertexList)
{
	D3DXVECTOR3 startPoint, endPoint;

	if (end.z < start.z)
	{
		startPoint = end;
		endPoint = start;
	}
	else
	{
		startPoint = start;
		endPoint = end;
	}

	D3DXVECTOR3 normal;

	// Draw out left quad

	vertexList[0].position.x = startPoint.x;
	vertexList[0].position.y = startPoint.y + halfThickness;
	vertexList[0].position.z = startPoint.z;
	vertexList[0].u = 1.0f;
	vertexList[0].v = 1.0f;

	vertexList[1].position.x = startPoint.x - halfThickness;
	vertexList[1].position.y = startPoint.y - halfThickness;
	vertexList[1].position.z = startPoint.z;
	vertexList[1].u = 0.0f;
	vertexList[1].v = 1.0f;

	vertexList[2].position.x = endPoint.x - halfThickness;
	vertexList[2].position.y = endPoint.y - halfThickness;
	vertexList[2].position.z = endPoint.z;
	vertexList[2].u = 0.0f;
	vertexList[2].v = 0.0f;

	vertexList[3].position.x = startPoint.x;
	vertexList[3].position.y = startPoint.y + halfThickness;
	vertexList[3].position.z = startPoint.z;
	vertexList[3].u = 1.0f;
	vertexList[3].v = 1.0f;

	vertexList[4].position.x = endPoint.x - halfThickness;
	vertexList[4].position.y = endPoint.y - halfThickness;
	vertexList[4].position.z = endPoint.z;
	vertexList[4].u = 0.0f;
	vertexList[4].v = 0.0f;

	vertexList[5].position.x = endPoint.x;
	vertexList[5].position.y = endPoint.y + halfThickness;
	vertexList[5].position.z = endPoint.z;
	vertexList[5].u = 1.0f;
	vertexList[5].v = 0.0f;

	// Now figure out left quad's normals
	D3DXVECTOR3 vec1, vec2;
	vec1 = vertexList[0].position - vertexList[1].position;
	vec2 = vertexList[2].position - vertexList[1].position;

	// normal is vec2 x vec1
	D3DXVec3Cross(&normal, &vec2, &vec1);

	vertexList[0].normal = D3DXVECTOR3(normal);
	vertexList[1].normal = D3DXVECTOR3(normal);
	vertexList[2].normal = D3DXVECTOR3(normal);
	vertexList[3].normal = D3DXVECTOR3(normal);
	vertexList[4].normal = D3DXVECTOR3(normal);
	vertexList[5].normal = D3DXVECTOR3(normal);



	// Draw out right quad
	vertexList[6].position.x = startPoint.x;
	vertexList[6].position.y = startPoint.y + halfThickness;
	vertexList[6].position.z = startPoint.z;
	vertexList[6].u = 0.0f;
	vertexList[6].v = 1.0f;

	vertexList[7].position.x = endPoint.x;
	vertexList[7].position.y = endPoint.y + halfThickness;
	vertexList[7].position.z = endPoint.z;
	vertexList[7].u = 0.0f;
	vertexList[7].v = 0.0f;

	vertexList[8].position.x = startPoint.x + halfThickness;
	vertexList[8].position.y = startPoint.y - halfThickness;
	vertexList[8].position.z = startPoint.z;
	vertexList[8].u = 1.0f;
	vertexList[8].v = 1.0f;

	vertexList[9].position.x = startPoint.x + halfThickness;
	vertexList[9].position.y = startPoint.y - halfThickness;
	vertexList[9].position.z = startPoint.z;
	vertexList[9].u = 1.0f;
	vertexList[9].v = 1.0f;

	vertexList[10].position.x = endPoint.x;
	vertexList[10].position.y = endPoint.y + halfThickness;
	vertexList[10].position.z = endPoint.z;
	vertexList[10].u = 0.0f;
	vertexList[10].v = 0.0f;

	vertexList[11].position.x = endPoint.x + halfThickness;
	vertexList[11].position.y = endPoint.y - halfThickness;
	vertexList[11].position.z = endPoint.z;
	vertexList[11].u = 1.0f;
	vertexList[11].v = 0.0f;

	// Now figure out right quad's normals
	vec1 = vertexList[6].position - vertexList[8].position;
	vec2 = vertexList[11].position - vertexList[8].position;

	// normal is vec1 x vec2
	D3DXVec3Cross(&normal, &vec1, &vec2);

	vertexList[6].normal = D3DXVECTOR3(normal);
	vertexList[7].normal = D3DXVECTOR3(normal);
	vertexList[8].normal = D3DXVECTOR3(normal);
	vertexList[9].normal = D3DXVECTOR3(normal);
	vertexList[10].normal = D3DXVECTOR3(normal);
	vertexList[11].normal = D3DXVECTOR3(normal);



	// Draw out bottom quad
	vertexList[12].position.x = startPoint.x - halfThickness;
	vertexList[12].position.y = startPoint.y - halfThickness;
	vertexList[12].position.z = startPoint.z;
	vertexList[12].u = 0.0f;
	vertexList[12].v = 0.0f;

	vertexList[13].position.x = startPoint.x + halfThickness;
	vertexList[13].position.y = startPoint.y - halfThickness;
	vertexList[13].position.z = startPoint.z;
	vertexList[13].u = 1.0f;
	vertexList[13].v = 0.0f;

	vertexList[14].position.x = endPoint.x - halfThickness;
	vertexList[14].position.y = endPoint.y - halfThickness;
	vertexList[14].position.z = endPoint.z;
	vertexList[14].u = 0.0f;
	vertexList[14].v = 1.0f;

	vertexList[15].position.x = startPoint.x + halfThickness;
	vertexList[15].position.y = startPoint.y - halfThickness;
	vertexList[15].position.z = startPoint.z;
	vertexList[15].u = 1.0f;
	vertexList[15].v = 0.0f;

	vertexList[16].position.x = endPoint.x + halfThickness;
	vertexList[16].position.y = endPoint.y - halfThickness;
	vertexList[16].position.z = endPoint.z;
	vertexList[16].u = 1.0f;
	vertexList[16].v = 1.0f;

	vertexList[17].position.x = endPoint.x - halfThickness;
	vertexList[17].position.y = endPoint.y - halfThickness;
	vertexList[17].position.z = endPoint.z;
	vertexList[17].u = 0.0f;
	vertexList[17].v = 1.0f;
	

	// Now figure out bottom quad's normals
	vec1 = vertexList[14].position - vertexList[13].position;
	vec2 = vertexList[16].position - vertexList[13].position;

	// normal is vec2 x vec1
	D3DXVec3Cross(&normal, &vec2, &vec1);

	vertexList[12].normal = D3DXVECTOR3(normal);
	vertexList[13].normal = D3DXVECTOR3(normal);
	vertexList[14].normal = D3DXVECTOR3(normal);
	vertexList[15].normal = D3DXVECTOR3(normal);
	vertexList[16].normal = D3DXVECTOR3(normal);
	vertexList[17].normal = D3DXVECTOR3(normal);
}


void LaserBeam::update(float seconds)
{
	currentTime += seconds;

	if (currentTime >= timeUntilFinished)
	{
		destroyed = true;
		return;
	}

	D3DXVECTOR3 ray = end - start;
	D3DXVec3Normalize(&ray, &ray);
	ray *= vanishingSpeed * seconds;
	start += ray;
}
