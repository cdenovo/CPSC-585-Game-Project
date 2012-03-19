#pragma once
#include "Mesh.h"
class RacerMesh :
	public Mesh
{
public:
	static RacerMesh* getInstance(IDirect3DDevice9* device);
	~RacerMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	RacerMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static RacerMesh* mesh;
};
