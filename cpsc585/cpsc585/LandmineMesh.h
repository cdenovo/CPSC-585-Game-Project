#pragma once
#include "Mesh.h"

class LandmineMesh :
	public Mesh
{
public:
	static LandmineMesh* getInstance(IDirect3DDevice9* device);
	~LandmineMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	LandmineMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static LandmineMesh* mesh;
};
