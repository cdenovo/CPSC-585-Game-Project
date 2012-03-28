#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Physics.h"

#include "Mesh.h"
#include "RacerMesh.h"
#include "WorldMesh.h"
#include "FrontWheelMesh.h"
#include "RearWheelMesh.h"
#include "WaypointMesh.h"
#include "LaserModel.h"

#include <string>


enum MeshType { RACER, TRAFFIC, WORLD, FRONTWHEEL, REARWHEEL, WAYPOINT, LASERMODEL };

class Drawable
{
public:
	Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device);
	~Drawable();
	void render(IDirect3DDevice9* device);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void setTransform(D3DXMATRIX* input);

	D3DXVECTOR3 getXVector();
	D3DXVECTOR3 getYVector();
	D3DXVECTOR3 getZVector();
	D3DXVECTOR3 getPosition();

	hkVector4 getXhkVector();
	hkVector4 getYhkVector();
	hkVector4 getZhkVector();

private:
	void initialize(MeshType type, std::string textureName, IDirect3DDevice9* device);


public:
	Mesh* mesh;

private:
	D3DXMATRIX transform;
	IDirect3DTexture9* texture;
};
