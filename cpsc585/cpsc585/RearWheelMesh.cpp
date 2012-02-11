#include "RearWheelMesh.h"


RearWheelMesh* RearWheelMesh::mesh = NULL;

RearWheelMesh* RearWheelMesh::getInstance(IDirect3DDevice9* device)
{
	if (mesh == NULL)
	{
		mesh = new RearWheelMesh(device);
	}

	return mesh;
}


RearWheelMesh::RearWheelMesh(IDirect3DDevice9* device)
{
	initialize(device);
}


RearWheelMesh::~RearWheelMesh(void)
{
}

void RearWheelMesh::render(IDirect3DDevice9* device)
{
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
	device->SetIndices(indexBuffer);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);
}


void RearWheelMesh::initialize(IDirect3DDevice9* device)
{
	Vertex* vertices;
	unsigned long* indices;

	vertexCount = 24;
	indexCount = 36;
	//vertices = new Vertex[vertexCount];
	//indices = new unsigned long[indexCount];

	device->CreateVertexBuffer(sizeof(Vertex) * vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2,
		D3DPOOL_MANAGED, &vertexBuffer, NULL);


	vertexBuffer->Lock(0, sizeof(Vertex) * vertexCount, (void**) &vertices, NULL);

	float tireHalfWidth = 0.4f;
	float tireRadius = 0.7f;
	
	Vertex tmp1[] =
     {
        { D3DXVECTOR3(-tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1, 0.5f},    // front
         { D3DXVECTOR3(tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.5f, 0.5f},
         { D3DXVECTOR3(-tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1, 0},
         { D3DXVECTOR3(tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.5f, 0},

         { D3DXVECTOR3(-tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.5f, 0.5f},    // back
         { D3DXVECTOR3(-tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.5f, 0},
         { D3DXVECTOR3(tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1, 0.5f},
         { D3DXVECTOR3(tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1, 0},

         { D3DXVECTOR3(-tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.5f, 0.5f},    // top
         { D3DXVECTOR3(-tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.5f, 0},
         { D3DXVECTOR3(tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1, 0.5f},
         { D3DXVECTOR3(tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1, 0},

         { D3DXVECTOR3(-tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.5f, 0},    // bottom
         { D3DXVECTOR3(tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1, 0},
         { D3DXVECTOR3(-tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.5f, 0.5f},
         { D3DXVECTOR3(tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1, 0.5f},

         { D3DXVECTOR3(tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0, 0.5f},    // right
         { D3DXVECTOR3(tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0, 0},
         { D3DXVECTOR3(tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.5f, 0.5f},
         { D3DXVECTOR3(tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.5f, 0},

         { D3DXVECTOR3(-tireHalfWidth, -tireRadius, -tireRadius), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.5f, 0.5f},    // left
         { D3DXVECTOR3(-tireHalfWidth, -tireRadius, tireRadius), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0, 0.5f},
         { D3DXVECTOR3(-tireHalfWidth, tireRadius, -tireRadius), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.5f, 0},
         { D3DXVECTOR3(-tireHalfWidth, tireRadius, tireRadius), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0, 0}
     };

	memcpy(vertices, tmp1, sizeof(tmp1));

	vertexBuffer->Unlock();

	device->CreateIndexBuffer(sizeof(unsigned long) * indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
		D3DPOOL_MANAGED, &indexBuffer, NULL);

	indexBuffer->Lock(0, sizeof(unsigned long) * indexCount, (void**) &indices, NULL);

	unsigned long tmp2[] =
     {
         0, 1, 2,    // side 1
         2, 1, 3,
         4, 5, 6,    // side 2
         6, 5, 7,
         8, 9, 10,    // side 3
         10, 9, 11,
         12, 13, 14,    // side 4
         14, 13, 15,
         16, 17, 18,    // side 5
         18, 17, 19,
         20, 21, 22,    // side 6
         22, 21, 23,
     };

	memcpy(indices, tmp2, sizeof(tmp2));

	indexBuffer->Unlock();
	

	return;
}
