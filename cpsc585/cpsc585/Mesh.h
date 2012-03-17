#pragma once

#define WIN32_LEAN_AND_MEAN

#include <d3dx9.h>
#include <string>
#include <iostream>
#include <fstream>

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);
	virtual void render(IDirect3DDevice9* device) = 0;

protected:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		float u, v; // Texture coordinates
	};

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9* indexBuffer;

	void loadMesh(std::string filename);

public:
	Vertex* vertices;
	unsigned long* indices;

	int vertexCount;
	int indexCount;
};
