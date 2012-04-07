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
#include "RocketMesh.h"
#include "LandmineMesh.h"

#include <string>


enum MeshType { RACER, TRAFFIC, WORLD, FRONTWHEEL, REARWHEEL, WAYPOINT, LASERMODEL, ROCKETMESH, LANDMINEMESH };

class Drawable
{
public:
	Drawable(void);
	Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device);
	virtual ~Drawable();
	virtual void render(IDirect3DDevice9* device);
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

	IDirect3DTexture9* getTexture();
	void setTexture(IDirect3DTexture9* tex);
	IDirect3DTexture9* getTextureFromFile(IDirect3DDevice9* device, std::string textureName);

	void buildShadowVolume(D3DXVECTOR3 light);
	void renderShadowVolume(IDirect3DDevice9* device);

private:
	void initialize(MeshType type, std::string textureName, IDirect3DDevice9* device);
	void addEdge(unsigned long* edges, int &numEdges, unsigned long v0, unsigned long v1);


public:
	Mesh* mesh;
	MeshType meshType;
	

protected:
	D3DXMATRIX transform;

private:
	IDirect3DTexture9* texture;

	IDirect3DVertexBuffer9* shadowVertexBuffer;
	int shadowVertCount;

	unsigned long* edges;
};
