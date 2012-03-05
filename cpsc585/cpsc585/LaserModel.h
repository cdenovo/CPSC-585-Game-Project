#pragma once
#include "Mesh.h"
class LaserModel :
	public Mesh
{
public:
	static LaserModel* getInstance(IDirect3DDevice9* device);
	~LaserModel(void);
	void render(IDirect3DDevice9* device);

protected:
	LaserModel(IDirect3DDevice9* device);

private:
	void initialize(IDirect3DDevice9* device);


public:
	bool drawLaser;

private:
	LaserModel* mesh;
};
