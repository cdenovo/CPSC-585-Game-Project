#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#ifndef _MODEL_H
#define _MODEL_H
class Model
{
public:
	Model(void);
	~Model(void);
	void initialize(IDirect3DDevice9* device);
	void shutdown();
	void render(IDirect3DDevice9* device, float posX, float posY, float posZ, int rotXDegrees, int rotYDegrees, int rotZDegrees);
	int getIndexCount();

private:
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
#endif