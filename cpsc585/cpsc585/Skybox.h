#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Mesh.h"

enum WallType { LEFTQUAD, RIGHTQUAD, TOPQUAD, BOTTOMQUAD, FRONTQUAD, BACKQUAD };

class Skybox
{
public:
	Skybox(IDirect3DDevice9* device);
	~Skybox();
	void render(IDirect3DDevice9* device);

private:
	void drawQuad(IDirect3DDevice9* device, IDirect3DTexture9* tex, IDirect3DVertexBuffer9* buffer);
	void makeQuad(float scale, WallType type,
		IDirect3DDevice9* device, IDirect3DVertexBuffer9* &buffer);


	IDirect3DTexture9* frontTex;
	IDirect3DTexture9* backTex;
	IDirect3DTexture9* leftTex;
	IDirect3DTexture9* rightTex;
	IDirect3DTexture9* bottomTex;
	IDirect3DTexture9* topTex;

	IDirect3DIndexBuffer9* indexBuffer;
	IDirect3DVertexBuffer9* frontBuffer;
	IDirect3DVertexBuffer9* backBuffer;
	IDirect3DVertexBuffer9* leftBuffer;
	IDirect3DVertexBuffer9* rightBuffer;
	IDirect3DVertexBuffer9* bottomBuffer;
	IDirect3DVertexBuffer9* topBuffer;
};

