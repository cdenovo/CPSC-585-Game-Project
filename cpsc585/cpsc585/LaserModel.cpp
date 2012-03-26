#include "LaserModel.h"


LaserModel* LaserModel::getInstance(IDirect3DDevice9* device)
{
	return new LaserModel(device);
}


LaserModel::LaserModel(IDirect3DDevice9* device)
{
	drawLaser = false;
	initialize(device);
}


LaserModel::~LaserModel(void)
{
}

void LaserModel::render(IDirect3DDevice9* device)
{
	if (!drawLaser)
		return;

	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
	device->SetIndices(indexBuffer);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);
}


void LaserModel::initialize(IDirect3DDevice9* device)
{
	D3DXVECTOR3 startPoint = D3DXVECTOR3(0.0f, 0.6f, 1.8f);
	D3DXVECTOR3 endPoint = D3DXVECTOR3(0.0f, 0.0f, 81.8f);
	D3DXVECTOR3 normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	float thickness = 0.2f; // half thickness, actually
	
	vertexCount = 6;
	vertices = new Vertex[6];
	vertices[0].position = D3DXVECTOR3(startPoint.x - thickness, startPoint.y, startPoint.z);
	vertices[0].normal = D3DXVECTOR3(normal);
	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;

	vertices[1].position = D3DXVECTOR3(startPoint.x + thickness, startPoint.y, startPoint.z);
	vertices[1].normal = D3DXVECTOR3(normal);
	vertices[1].u = 1.0f;
	vertices[1].v = 1.0f;

	vertices[2].position = D3DXVECTOR3(endPoint.x, endPoint.y, endPoint.z);
	vertices[2].normal = D3DXVECTOR3(normal);
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;

	vertices[3].position = D3DXVECTOR3(startPoint.x + thickness, startPoint.y, startPoint.z);
	vertices[3].normal = D3DXVECTOR3(normal);
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	vertices[4].position = D3DXVECTOR3(endPoint.x, endPoint.y, endPoint.z);
	vertices[4].normal = D3DXVECTOR3(normal);
	vertices[4].u = 1.0f;
	vertices[4].v = 0.0f;

	vertices[5].position = D3DXVECTOR3(endPoint.x, endPoint.y, endPoint.z);
	vertices[5].normal = D3DXVECTOR3(normal);
	vertices[5].u = 0.0f;
	vertices[5].v = 0.0f;

	indexCount = 6;
	indices = new unsigned long[6];
	for (int i = 0; i < 6; i++)
	{
		indices[i] = 5 - i;
	}

	float* verts;
	unsigned long* inds;

	device->CreateVertexBuffer(sizeof(Vertex) * vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2,
		D3DPOOL_MANAGED, &vertexBuffer, NULL);


	vertexBuffer->Lock(0, sizeof(Vertex) * vertexCount, (void**) &verts, NULL);

	memcpy(verts, vertices, sizeof(Vertex) * vertexCount);

	vertexBuffer->Unlock();

	device->CreateIndexBuffer(sizeof(unsigned long) * indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
		D3DPOOL_MANAGED, &indexBuffer, NULL);

	indexBuffer->Lock(0, sizeof(unsigned long) * indexCount, (void**) &inds, NULL);

	memcpy(inds, indices, sizeof(unsigned long) * indexCount);

	indexBuffer->Unlock();

	return;
}
