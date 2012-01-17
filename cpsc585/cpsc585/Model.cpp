#include "Model.h"


Model::Model(void)
{
	vertexBuffer = NULL;
	indexBuffer = NULL;
}


Model::~Model(void)
{
}

int Model::getIndexCount()
{
	return indexCount;
}

void Model::initialize(IDirect3DDevice9* device)
{
	Vertex* vertices;
	unsigned long* indices;

	vertexCount = 3;
	indexCount = 3;
	vertices = new Vertex[vertexCount];
	indices = new unsigned long[indexCount];

	device->CreateVertexBuffer(sizeof(Vertex) * vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED, &vertexBuffer, NULL);


	vertexBuffer->Lock(0, sizeof(Vertex) * vertexCount, (void**) &vertices, NULL);

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = D3DCOLOR_XRGB(0,255,0);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = D3DCOLOR_XRGB(255,0,0);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = D3DCOLOR_XRGB(0,255,0);

	vertexBuffer->Unlock();

	device->CreateIndexBuffer(sizeof(unsigned long) * indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
		D3DPOOL_MANAGED, &indexBuffer, NULL);

	indexBuffer->Lock(0, sizeof(unsigned long) * indexCount, (void**) &indices, NULL);

	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	indexBuffer->Unlock();
	

	return;
}

void Model::shutdown()
{
	if (indexBuffer)
	{
		indexBuffer = NULL;
	}

	if (vertexBuffer)
	{
		vertexBuffer = NULL;
	}

	return;
}


void Model::render(IDirect3DDevice9* device)
{
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->SetIndices(indexBuffer);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	
	return;
}
