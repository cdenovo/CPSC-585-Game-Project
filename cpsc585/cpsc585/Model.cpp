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

	vertexCount = 24;
	indexCount = 36;
	//vertices = new Vertex[vertexCount];
	//indices = new unsigned long[indexCount];

	device->CreateVertexBuffer(sizeof(Vertex) * vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL,
		D3DPOOL_MANAGED, &vertexBuffer, NULL);


	vertexBuffer->Lock(0, sizeof(Vertex) * vertexCount, (void**) &vertices, NULL);

	
	Vertex tmp1[] =
     {
        { D3DXVECTOR3(-3.0f, -3.0f, 3.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f)},    // side 1
         { D3DXVECTOR3(3.0f, -3.0f, 3.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f)},
         { D3DXVECTOR3(-3.0f, 3.0f, 3.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f)},
         { D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f)},

         { D3DXVECTOR3(-3.0f, -3.0f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f)},    // side 2
         { D3DXVECTOR3(-3.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f)},
         { D3DXVECTOR3(3.0f, -3.0f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f)},
         { D3DXVECTOR3(3.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f)},

         { D3DXVECTOR3(-3.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},    // side 3
         { D3DXVECTOR3(-3.0f, 3.0f, 3.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},
         { D3DXVECTOR3(3.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},
         { D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},

         { D3DXVECTOR3(-3.0f, -3.0f, -3.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f)},    // side 4
         { D3DXVECTOR3(3.0f, -3.0f, -3.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f)},
         { D3DXVECTOR3(-3.0f, -3.0f, 3.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f)},
         { D3DXVECTOR3(3.0f, -3.0f, 3.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f)},

         { D3DXVECTOR3(3.0f, -3.0f, -3.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f)},    // side 5
         { D3DXVECTOR3(3.0f, 3.0f, -3.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f)},
         { D3DXVECTOR3(3.0f, -3.0f, 3.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f)},
         { D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f)},

         { D3DXVECTOR3(-3.0f, -3.0f, -3.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f)},    // side 6
         { D3DXVECTOR3(-3.0f, -3.0f, 3.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f)},
         { D3DXVECTOR3(-3.0f, 3.0f, -3.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f)},
         { D3DXVECTOR3(-3.0f, 3.0f, 3.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f)},
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


void Model::render(IDirect3DDevice9* device, float posX, float posY, float posZ, int rotXDegrees, int rotYDegrees, int rotZDegrees)
{
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
	device->SetIndices(indexBuffer);
	
	// Convert to radians
	float rotX, rotY, rotZ;

	D3DXMATRIX trans, worldMatrix;

	D3DXMatrixIdentity(&worldMatrix);

	if (rotXDegrees != 0)
	{
		rotX = 0.0174f * (float) rotXDegrees;
		D3DXMatrixRotationX(&trans, rotX);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &trans);
	}

	if (rotYDegrees != 0)
	{
		rotY = 0.0174f * (float) rotYDegrees;
		D3DXMatrixRotationY(&trans, rotY);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &trans);
	}

	if (rotZDegrees != 0)
	{
		rotZ = 0.0174f * (float) rotZDegrees;
		D3DXMatrixRotationZ(&trans, rotZ);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &trans);
	}


	D3DXMatrixTranslation(&trans, posX, posY, posZ);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &trans);

	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);
	
	
	return;
}
