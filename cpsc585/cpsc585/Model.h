#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

class Model
{
public:
	Model(void);
	~Model(void);
	void initialize(IDirect3DDevice9* device);
	void shutdown();
	void render(IDirect3DDevice9* device);
	int getIndexCount();

private:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
	};

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9* indexBuffer;
	int vertexCount;
	int indexCount;
};

