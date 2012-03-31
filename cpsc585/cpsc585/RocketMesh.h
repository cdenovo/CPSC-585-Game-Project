#pragma once
#include "Mesh.h"

class RocketMesh :
	public Mesh
{
public:
	static RocketMesh* getInstance(IDirect3DDevice9* device);
	~RocketMesh(void);
	void render(IDirect3DDevice9* device);

protected:
	RocketMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static RocketMesh* mesh;
};
