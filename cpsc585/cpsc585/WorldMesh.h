#pragma once
#include "Mesh.h"
class WorldMesh :
	public Mesh
{
public:
	static WorldMesh* getInstance(IDirect3DDevice9* device);
	~WorldMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	WorldMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static WorldMesh* mesh;
};
