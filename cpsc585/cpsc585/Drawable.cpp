#include "Drawable.h"


Drawable::Drawable(void)
{
	texture = NULL;
	edges = NULL;
}


Drawable::Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	D3DXMatrixIdentity(&transform);
	meshType = type;
	shadowVertexBuffer = NULL;
	edges = NULL;
	shadowVertCount = 0;
	initialize(type, textureName, device);
}


Drawable::~Drawable(void)
{
	if (texture)
	{
		texture->Release();
		texture = NULL;
	}

	if (edges)
	{
		delete [] edges;
	}
}


void Drawable::initialize(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	texture = NULL;
	edges = NULL;

	D3DXCreateTextureFromFile(device, textureName.c_str(), &texture);

	switch (type)
	{
	case RACER:
		{
			mesh = RacerMesh::getInstance(device);

			// Racers have shadows: set up vertex & index buffers
			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			edges = new unsigned long[mesh->indexCount * 2];

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

			// Wheels have shadows: set up vertex & index buffers
			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			edges = new unsigned long[mesh->indexCount * 2];

			break;
		}
	case REARWHEEL:
		{
			mesh = RearWheelMesh::getInstance(device);

			// Wheels have shadows: set up vertex & index buffers
			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			edges = new unsigned long[mesh->indexCount * 2];

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


IDirect3DTexture9* Drawable::getTexture()
{
	return texture;
}

void Drawable::setTexture(IDirect3DTexture9* tex)
{
	texture = tex;
}


IDirect3DTexture9* Drawable::getTextureFromFile(IDirect3DDevice9* device, std::string textureName)
{
	IDirect3DTexture9* tex = NULL;
	D3DXCreateTextureFromFile(device, textureName.c_str(), &tex);
	return tex;
}


void Drawable::buildShadowVolume(D3DXVECTOR3 light)
{
	// Got most of this (and the addEdge() function) from some
	// sample source code online, which said it got most of
	// the code in turn from the DirectX SDK

	D3DXMATRIX invTrans;
	D3DXQUATERNION rot;
	D3DXVECTOR3 scale, translate;
	D3DXMatrixDecompose(&scale, &rot, &scale, &transform);
	D3DXQuaternionInverse(&rot, &rot);
	D3DXVECTOR4 temp;
	D3DXMatrixRotationQuaternion(&invTrans, &rot);
	D3DXVec3Transform(&temp, &light, &invTrans);
	light.x = -temp.x;
	light.y = -temp.y;
	light.z = -temp.z;
	//D3DXVec3Normalize(&light, &light);

	Vertex* vertices = mesh->vertices;
	unsigned long* indices = mesh->indices;
	int vertexCount = mesh->vertexCount;
	int indexCount = mesh->indexCount;

	int numFaces = indexCount / 3;

	ZeroMemory(edges, numFaces * 6 * sizeof(unsigned long));

	int numEdges = 0;

	unsigned long index0, index1, index2;

	for (int i = 0; i < numFaces; ++i)
	{
		index0 = indices[3*i];
		index1 = indices[3*i+1];
		index2 = indices[3*i+2];

		D3DXVECTOR3 v0 = vertices[index0].position;
        D3DXVECTOR3 v1 = vertices[index1].position;
        D3DXVECTOR3 v2 = vertices[index2].position;

		D3DXVECTOR3 norm = vertices[index0].normal + vertices[index1].normal + vertices[index2].normal;
		norm /= 3.0f;

		if (D3DXVec3Dot(&norm, &light) > 0.0f)
		{
			addEdge(edges, numEdges, index0, index1);
			addEdge(edges, numEdges, index1, index2);
			addEdge(edges, numEdges, index2, index0);
		}
	}


	D3DXVECTOR3* points;

	shadowVertexBuffer->Lock(0, sizeof(D3DXVECTOR3) * mesh->indexCount * 6, (void**) &points, NULL);

	int numVertices = 0;
	D3DXVECTOR3 v0, v1, v2, v3;


	for (int i = 0; i < numEdges; i++)
	{
		v0 = vertices[edges[2*i]].position;
		v1 = vertices[edges[2*i + 1]].position;
		v2 = v0 - light*800;
		v3 = v1 - light*800;

		points[numVertices++] = v0;
		points[numVertices++] = v1;
		points[numVertices++] = v2;

		points[numVertices++] = v1;
		points[numVertices++] = v3;
		points[numVertices++] = v2;
	}

	shadowVertexBuffer->Unlock();

	shadowVertCount = numVertices;
}


void Drawable::renderShadowVolume(IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, &transform);
	device->SetStreamSource(0, shadowVertexBuffer, 0, sizeof(D3DXVECTOR3));
	device->SetFVF(D3DFVF_XYZ);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, shadowVertCount / 3);
}

void Drawable::addEdge(unsigned long* edges, int &numEdges, unsigned long v0, unsigned long v1)
{
    for (int i = 0; i < numEdges; i++)
    {
        if ((edges[2*i] == v0 && edges[2*i + 1] == v1 ) ||
            (edges[2*i + 0] == v1 && edges[2*i +1 ] == v0 ))
        {
            if(numEdges > 1)
            {
                edges[2*i] = edges[2*(numEdges-1)];
                edges[2*i + 1] = edges[2*(numEdges-1) + 1];
            }

            numEdges--;

            return;
        }
    }

    edges[2*numEdges] = v0;
    edges[2*numEdges + 1] = v1;

    numEdges++;
}
