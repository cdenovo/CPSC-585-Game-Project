#pragma once
#include "Mesh.h"
class FrontWheelMesh :
	public Mesh
{
public:
	static FrontWheelMesh* getInstance(IDirect3DDevice9* device);
	~FrontWheelMesh(void);
	void render(IDirect3DDevice9* device);
protected:
	FrontWheelMesh(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:

private:
	static FrontWheelMesh* mesh;
};

