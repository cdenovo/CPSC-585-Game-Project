#include "Drawable.h"


Drawable::Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	D3DXMatrixIdentity(&transform);
	initialize(type, textureName, device);
}


Drawable::~Drawable(void)
{
}


void Drawable::initialize(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	// set up private texture member, loading texture textureName

	switch (type)
	{
	case RACER:
		{
			mesh = RacerMesh::getInstance(device);
			break;
		}
	case WORLD:
		{
			mesh = RacerMesh::getInstance(device);
			break;
		}
	default:
		mesh = NULL;
	}

}


void Drawable::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)
{
	D3DXMATRIX temp;

	D3DXMatrixIdentity(&transform);
	
	
	D3DXMatrixRotationX(&temp, rotX);
	D3DXMatrixMultiply(&transform, &transform, &temp);
	D3DXMatrixRotationY(&temp, rotY);
	D3DXMatrixMultiply(&transform, &transform, &temp);
	D3DXMatrixRotationZ(&temp, rotZ);
	D3DXMatrixMultiply(&transform, &transform, &temp);

	D3DXMatrixTranslation(&temp, posX, posY, posZ);
	D3DXMatrixMultiply(&transform, &transform, &temp);
}

void Drawable::render(IDirect3DDevice9* device)
{
	// Apply transforms, THEN call render on the mesh
	device->SetTransform(D3DTS_WORLD, &transform);

	mesh->render(device);
}

void Drawable::setTransform(D3DXMATRIX* input)
{
	D3DXMatrixIdentity(&transform);
	D3DXMatrixMultiply(&transform, &transform, input);
}
