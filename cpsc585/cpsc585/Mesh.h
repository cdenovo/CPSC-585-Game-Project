#pragma once

#include <d3dx9.h>

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
	};

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9* indexBuffer;
	int vertexCount;
	int indexCount;


};

