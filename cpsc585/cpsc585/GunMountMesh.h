#pragma once
#include "Mesh.h"
class GunMountMesh :
	public Mesh
{
public:
	static GunMountMesh* getInstance(IDirect3DDevice9* device);
	~GunMountMesh();
	void render(IDirect3DDevice9* device);

protected:
	GunMountMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static GunMountMesh* mesh;
};
