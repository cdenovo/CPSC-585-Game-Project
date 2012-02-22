#pragma once
#include "Mesh.h"
class RearWheelMesh :
	public Mesh
{
public:
	static RearWheelMesh* getInstance(IDirect3DDevice9* device);
	~RearWheelMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	RearWheelMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static RearWheelMesh* mesh;
};
