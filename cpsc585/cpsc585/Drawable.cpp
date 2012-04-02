#include "Drawable.h"


Drawable::Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	D3DXMatrixIdentity(&transform);
	initialize(type, textureName, device);
}


Drawable::~Drawable(void)
{
	if (texture)
	{
		texture->Release();
		texture = NULL;
	}
}


void Drawable::initialize(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	texture = NULL;

	D3DXCreateTextureFromFile(device, textureName.c_str(), &texture);

	switch (type)
	{
	case RACER:
		{
			mesh = RacerMesh::getInstance(device);
			break;
		}
	case WORLD:
		{
			mesh = WorldMesh::getInstance(device);
			break;
		}
	case FRONTWHEEL:
		{
			mesh = FrontWheelMesh::getInstance(device);
			break;
		}
	case REARWHEEL:
		{
			mesh = RearWheelMesh::getInstance(device);
			break;
		}
	case WAYPOINT:
		{
			mesh = WaypointMesh::getInstance(device);
			break;
		}
	case LASERMODEL:
		{
			mesh = LaserModel::getInstance(device);
			break;
		}
	case ROCKETMESH:
		{
			mesh = RocketMesh::getInstance(device);
			break;
		}
	case LANDMINEMESH:
		{
			mesh = LandmineMesh::getInstance(device);
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
	device->SetTexture(0, texture);

	mesh->render(device);
}

void Drawable::setTransform(D3DXMATRIX* input)
{
	D3DXMatrixIdentity(&transform);
	D3DXMatrixMultiply(&transform, &transform, input);
}


D3DXVECTOR3 Drawable::getXVector()
{
	return D3DXVECTOR3(transform._11, transform._12, transform._13);
}

D3DXVECTOR3 Drawable::getYVector()
{
	return D3DXVECTOR3(transform._21, transform._22, transform._23);
}

D3DXVECTOR3 Drawable::getZVector()
{
	return D3DXVECTOR3(transform._31, transform._32, transform._33);
}

D3DXVECTOR3 Drawable::getPosition()
{
	return D3DXVECTOR3(transform._41, transform._42, transform._43);
}

hkVector4 Drawable::getXhkVector()
{
	return hkVector4(transform._11, transform._12, transform._13);
}

hkVector4 Drawable::getYhkVector()
{
	return hkVector4(transform._21, transform._22, transform._23);
}

hkVector4 Drawable::getZhkVector()
{
	return hkVector4(transform._31, transform._32, transform._33);
}
