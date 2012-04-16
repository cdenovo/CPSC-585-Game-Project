#pragma once
#include "Mesh.h"
class GunMesh :
	public Mesh
{
public:
	static GunMesh* getInstance(IDirect3DDevice9* device);
	~GunMesh();
	void render(IDirect3DDevice9* device);

protected:
	GunMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static GunMesh* mesh;
};
