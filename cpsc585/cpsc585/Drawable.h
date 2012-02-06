#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Mesh.h"
#include "RacerMesh.h"

#include <string>


enum MeshType { RACER, TRAFFIC, WORLD };

class Drawable
{
public:
	Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device);
	~Drawable(void);
	void render(IDirect3DDevice9* device);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void setTransform(D3DXMATRIX* input);
	

private:
	void initialize(MeshType type, std::string textureName, IDirect3DDevice9* device);


public:

private:
	Mesh* mesh;
	D3DXMATRIX transform;
	// need to add texture member
};
