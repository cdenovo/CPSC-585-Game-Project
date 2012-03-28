#include "Skybox.h"


Skybox::Skybox(IDirect3DDevice9* device)
{
	frontTex = NULL;
	backTex = NULL;
	leftTex = NULL;
	rightTex = NULL;
	topTex = NULL;
	bottomTex = NULL;

	frontBuffer = NULL;
	backBuffer = NULL;
	leftBuffer = NULL;
	rightBuffer = NULL;
	topBuffer = NULL;
	bottomBuffer = NULL;



	unsigned long* indices = new unsigned long[6];
	for (int i = 0; i < 6; i++)
	{
		indices[i] = i;
	}

	unsigned long* inds;

	device->CreateIndexBuffer(sizeof(unsigned long) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
		D3DPOOL_MANAGED, &indexBuffer, NULL);

	indexBuffer->Lock(0, sizeof(unsigned long) * 6, (void**) &inds, NULL);

	memcpy(inds, indices, sizeof(unsigned long) * 6);

	indexBuffer->Unlock();

	D3DXCreateTextureFromFile(device, "front.dds", &frontTex);
	D3DXCreateTextureFromFile(device, "back.dds", &backTex);
	D3DXCreateTextureFromFile(device, "left.dds", &leftTex);
	D3DXCreateTextureFromFile(device, "right.dds", &rightTex);
	D3DXCreateTextureFromFile(device, "top.dds", &topTex);
	D3DXCreateTextureFromFile(device, "bottom.dds", &bottomTex);

	float scale = 2.0f;

	makeQuad(scale,FRONTQUAD, device, frontBuffer);
	makeQuad(scale,BACKQUAD, device, backBuffer);
	makeQuad(scale,LEFTQUAD, device, leftBuffer);
	makeQuad(scale,RIGHTQUAD, device, rightBuffer);
	makeQuad(scale,TOPQUAD, device, topBuffer);
	makeQuad(scale,BOTTOMQUAD, device, bottomBuffer);
}


Skybox::~Skybox()
{
	if (frontTex)
	{
		frontTex->Release();
		frontTex = NULL;
	}

	if (backTex)
	{
		backTex->Release();
		backTex = NULL;
	}

	if (leftTex)
	{
		leftTex->Release();
		leftTex = NULL;
	}

	if (rightTex)
	{
		rightTex->Release();
		rightTex = NULL;
	}

	if (topTex)
	{
		topTex->Release();
		topTex = NULL;
	}

	if (bottomTex)
	{
		bottomTex->Release();
		bottomTex = NULL;
	}
}


void Skybox::render(IDirect3DDevice9* device)
{
	drawQuad(device, frontTex, frontBuffer);
	drawQuad(device, backTex, backBuffer);
	drawQuad(device, leftTex, leftBuffer);
	drawQuad(device, rightTex, rightBuffer);
	drawQuad(device, topTex, topBuffer);
	drawQuad(device, bottomTex, bottomBuffer);
}


void Skybox::drawQuad(IDirect3DDevice9* device, IDirect3DTexture9* tex, IDirect3DVertexBuffer9* buffer)
{
	device->SetTexture(0, tex);
	device->SetStreamSource(0, buffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
	device->SetIndices(indexBuffer);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
}


void Skybox::makeQuad(float scale, WallType type,
	IDirect3DDevice9* device, IDirect3DVertexBuffer9* &buffer)
{
	int constantAxis, widthAxis, heightAxis;

	if (type < 2)
	{
		constantAxis = 0;
		widthAxis = 2;
		heightAxis = 1;
	}
	else if (type < 4)
	{
		constantAxis = 1;
		widthAxis = 0;
		heightAxis = 2;
	}
	else
	{
		constantAxis = 2;
		widthAxis = 0;
		heightAxis = 1;
	}


	D3DXVECTOR3 normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float flip = 1.0f;

	if (type == FRONTQUAD)
	{
		normal.z = -1.0f;
	}
	else if (type == BACKQUAD)
	{
		normal.z = 1.0f;
		flip = -1.0f;
	}
	else if (type == RIGHTQUAD)
	{
		normal.x = -1.0f;
		flip = -1.0f;
	}
	else if (type == LEFTQUAD)
	{
		normal.x = 1.0f;
	}
	else if (type == TOPQUAD)
	{
		normal.y = -1.0f;
		flip = -1.0f;
	}
	else if (type == BOTTOMQUAD)
	{
		normal.y = 1.0f;
	}

	int vertexCount = 6;
	Vertex* vertices = new Vertex[6];
	
	vertices[0].position = D3DXVECTOR3(-normal);
	vertices[0].position[widthAxis] = 1.0f;
	vertices[0].position[heightAxis] = -1.0f * flip;
	vertices[0].position *= scale;
	vertices[0].normal = D3DXVECTOR3(normal);
	vertices[0].u = 1.0f;
	vertices[0].v = 1.0f;

	if (flip < 0.0f)
	{
		vertices[0].v = 0.0f;
	}

	vertices[1].position = D3DXVECTOR3(-normal);
	vertices[1].position[widthAxis] = -1.0f;
	vertices[1].position[heightAxis] = -1.0f;
	vertices[1].position *= scale;
	vertices[1].normal = D3DXVECTOR3(normal);
	vertices[1].u = 0.0f;
	vertices[1].v = 1.0f;

	vertices[2].position = D3DXVECTOR3(-normal);
	vertices[2].position[widthAxis] = 1.0f;
	vertices[2].position[heightAxis] = 1.0f * flip;
	vertices[2].position *= scale;
	vertices[2].normal = D3DXVECTOR3(normal);
	vertices[2].u = 1.0f;
	vertices[2].v = 0.0f;

	if (flip < 0.0f)
	{
		vertices[2].v = 1.0f;
	}

	vertices[3].position = D3DXVECTOR3(-normal);
	vertices[3].position[widthAxis] = 1.0f * flip;
	vertices[3].position[heightAxis] = 1.0f;
	vertices[3].position *= scale;
	vertices[3].normal = D3DXVECTOR3(normal);
	vertices[3].u = 1.0f;
	vertices[3].v = 0.0f;

	if (flip < 0.0f)
	{
		vertices[3].u = 0.0f;
	}

	vertices[4].position = D3DXVECTOR3(-normal);
	vertices[4].position[widthAxis] = -1.0f;
	vertices[4].position[heightAxis] = -1.0f;
	vertices[4].position *= scale;
	vertices[4].normal = D3DXVECTOR3(normal);
	vertices[4].u = 0.0f;
	vertices[4].v = 1.0f;

	vertices[5].position = D3DXVECTOR3(-normal);
	vertices[5].position[widthAxis] = -1.0f * flip;
	vertices[5].position[heightAxis] = 1.0f;
	vertices[5].position *= scale;
	vertices[5].normal = D3DXVECTOR3(normal);
	vertices[5].u = 0.0f;
	vertices[5].v = 0.0f;

	if (flip < 0.0f)
	{
		vertices[5].u = 1.0f;
	}

	float* verts;

	device->CreateVertexBuffer(sizeof(Vertex) * vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2,
		D3DPOOL_MANAGED, &buffer, NULL);


	buffer->Lock(0, sizeof(Vertex) * vertexCount, (void**) &verts, NULL);

	memcpy(verts, vertices, sizeof(Vertex) * vertexCount);

	buffer->Unlock();
}
