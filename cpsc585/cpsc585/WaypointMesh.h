#pragma once
#include "Mesh.h"
class WaypointMesh :
	public Mesh
{
public:
	static WaypointMesh* getInstance(IDirect3DDevice9* device);
	~WaypointMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	WaypointMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static WaypointMesh* mesh;
};
