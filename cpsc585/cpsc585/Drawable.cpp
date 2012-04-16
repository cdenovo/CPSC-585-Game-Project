#include "Drawable.h"

unsigned long** Drawable::racerConnectivityTable = NULL;
unsigned long** Drawable::frontWheelConnectivityTable = NULL;
unsigned long** Drawable::rearWheelConnectivityTable = NULL;
unsigned long** Drawable::gunConnectivityTable = NULL;
unsigned long** Drawable::gunMountConnectivityTable = NULL;


Drawable::Drawable(void)
{
	texture = NULL;
}


Drawable::Drawable(MeshType type, std::string textureName, IDirect3DDevice9* device)
{
	D3DXMatrixIdentity(&transform);
	meshType = type;
	shadowVertexBuffer = NULL;
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

			// Racers have shadows: set up vertex & index buffers
			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			if (!racerConnectivityTable)
			{
				int numFaces = mesh->indexCount / 3;
				racerConnectivityTable = new unsigned long*[numFaces];

				for (int i = 0; i < numFaces; i++)
				{
					racerConnectivityTable[i] = new unsigned long[3];
					ZeroMemory(racerConnectivityTable[i], sizeof(unsigned long) * 3);
				}

				Vertex* vertices = mesh->vertices;
				unsigned long* indices = mesh->indices;
				unsigned long indexCount = mesh->indexCount;

				unsigned long v0, v1, v2;

				// Now set up connectivity table
				// (MUCH faster than classical approach for stencil shadows!)

				// For each triangle...
				for (int i = 0; i < numFaces; i++)
				{
					// For each edge:
					// Search index list. Add triangle index to connectivity table when the edge in reverse order is found
					v0 = indices[i * 3];
					v1 = indices[i * 3 + 1];
					v2 = indices[i * 3 + 2];

					// First edge = (v0, v1). Searching for (v1, v0) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
								{
									racerConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
								{
									racerConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3]].position == vertices[v0].position)
								{
									racerConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
						}
					}

					// Second edge = (v1, v2). Searching for (v2, v1) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
								{
									racerConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
								{
									racerConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3]].position == vertices[v1].position)
								{
									racerConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
						}
					}
					
					// Third edge = (v2, v0). Searching for (v0, v2) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
								{
									racerConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
								{
									racerConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3]].position == vertices[v2].position)
								{
									racerConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
						}
					}

				}
			}

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

			if (!frontWheelConnectivityTable)
			{
				int numFaces = mesh->indexCount / 3;
				frontWheelConnectivityTable = new unsigned long*[numFaces];

				for (int i = 0; i < numFaces; i++)
				{
					frontWheelConnectivityTable[i] = new unsigned long[3];
					ZeroMemory(frontWheelConnectivityTable[i], sizeof(unsigned long) * 3);
				}

				Vertex* vertices = mesh->vertices;
				unsigned long* indices = mesh->indices;
				unsigned long indexCount = mesh->indexCount;

				unsigned long v0, v1, v2;

				// Now set up connectivity table
				// (MUCH faster than classical approach for stencil shadows!)

				// For each triangle...
				for (int i = 0; i < numFaces; i++)
				{
					if (i == 44)
					{
						int jkfldfjsd = 0;
					}

					// For each edge:
					// Search index list. Add triangle index to connectivity table when the edge in reverse order is found
					v0 = indices[i * 3];
					v1 = indices[i * 3 + 1];
					v2 = indices[i * 3 + 2];

					// First edge = (v0, v1). Searching for (v1, v0) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
								{
									frontWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
								{
									frontWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3]].position == vertices[v0].position)
								{
									frontWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
						}
					}


					// Second edge = (v1, v2). Searching for (v2, v1) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
								{
									frontWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
								{
									frontWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3]].position == vertices[v1].position)
								{
									frontWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
						}
					}
					
					// Third edge = (v2, v0). Searching for (v0, v2) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
								{
									frontWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
								{
									frontWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3]].position == vertices[v2].position)
								{
									frontWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
						}
					}

				}
			}

			break;
		}
	case REARWHEEL:
		{
			mesh = RearWheelMesh::getInstance(device);
			
			// Wheels have shadows: set up vertex & index buffers
			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			if (!rearWheelConnectivityTable)
			{
				int numFaces = mesh->indexCount / 3;
				rearWheelConnectivityTable = new unsigned long*[numFaces];

				for (int i = 0; i < numFaces; i++)
				{
					rearWheelConnectivityTable[i] = new unsigned long[3];
					ZeroMemory(rearWheelConnectivityTable[i], sizeof(unsigned long) * 3);
				}

				Vertex* vertices = mesh->vertices;
				unsigned long* indices = mesh->indices;
				unsigned long indexCount = mesh->indexCount;

				unsigned long v0, v1, v2;

				// Now set up connectivity table
				// (MUCH faster than classical approach for stencil shadows!)

				// For each triangle...
				for (int i = 0; i < numFaces; i++)
				{
					// For each edge:
					// Search index list. Add triangle index to connectivity table when the edge in reverse order is found
					v0 = indices[i * 3];
					v1 = indices[i * 3 + 1];
					v2 = indices[i * 3 + 2];

					// First edge = (v0, v1). Searching for (v1, v0) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
								{
									rearWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
								{
									rearWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3]].position == vertices[v0].position)
								{
									rearWheelConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
						}
					}

					// Second edge = (v1, v2). Searching for (v2, v1) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
								{
									rearWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
								{
									rearWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3]].position == vertices[v1].position)
								{
									rearWheelConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
						}
					}
					
					// Third edge = (v2, v0). Searching for (v0, v2) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
								{
									rearWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
								{
									rearWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3]].position == vertices[v2].position)
								{
									rearWheelConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
						}
					}

				}
			}

			break;
		}
	case WAYPOINT:
		{
			mesh = WaypointMesh::getInstance(device);
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
	case GUNMESH:
		{
			mesh = GunMesh::getInstance(device);
			break;
		}
	case GUNMOUNTMESH:
		{
			mesh = GunMountMesh::getInstance(device);

			device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * mesh->indexCount * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZ,
				D3DPOOL_DEFAULT, &shadowVertexBuffer, NULL);

			if (!gunMountConnectivityTable)
			{
				int numFaces = mesh->indexCount / 3;
				gunMountConnectivityTable = new unsigned long*[numFaces];

				for (int i = 0; i < numFaces; i++)
				{
					gunMountConnectivityTable[i] = new unsigned long[3];
					ZeroMemory(gunMountConnectivityTable[i], sizeof(unsigned long) * 3);
				}
				
				Vertex* vertices = mesh->vertices;
				unsigned long* indices = mesh->indices;
				unsigned long indexCount = mesh->indexCount;

				unsigned long v0, v1, v2;

				// Now set up connectivity table
				// (MUCH faster than classical approach for stencil shadows!)

				// For each triangle...
				for (int i = 0; i < numFaces; i++)
				{
					// For each edge:
					// Search index list. Add triangle index to connectivity table when the edge in reverse order is found
					v0 = indices[i * 3];
					v1 = indices[i * 3 + 1];
					v2 = indices[i * 3 + 2];

					// First edge = (v0, v1). Searching for (v1, v0) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
								{
									gunMountConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
								{
									gunMountConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
							{
								if (vertices[indices[j*3]].position == vertices[v0].position)
								{
									gunMountConnectivityTable[i][0] = (unsigned long) j;
									break;
								}
							}
						}
					}

					// Second edge = (v1, v2). Searching for (v2, v1) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v1].position)
								{
									gunMountConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v1].position)
								{
									gunMountConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
							{
								if (vertices[indices[j*3]].position == vertices[v1].position)
								{
									gunMountConnectivityTable[i][1] = (unsigned long) j;
									break;
								}
							}
						}
					}
					
					// Third edge = (v2, v0). Searching for (v0, v2) in index list
					for (int j = 0; j < numFaces; j++)
					{
						if (j != i)
						{
							if (vertices[indices[j*3]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 1]].position == vertices[v2].position)
								{
									gunMountConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 1]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3 + 2]].position == vertices[v2].position)
								{
									gunMountConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
							else if (vertices[indices[j*3 + 2]].position == vertices[v0].position)
							{
								if (vertices[indices[j*3]].position == vertices[v2].position)
								{
									gunMountConnectivityTable[i][2] = (unsigned long) j;
									break;
								}
							}
						}
					}

				}
			}

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
	// Use connectivity table to determine silhouette edges
	unsigned long** connectivityTable;

	switch (meshType)
	{
	case (RACER):
		{
			connectivityTable = racerConnectivityTable;
			break;
		}
	case (FRONTWHEEL):
		{
			connectivityTable = frontWheelConnectivityTable;
			break;
		}
	case (REARWHEEL):
		{
			connectivityTable = rearWheelConnectivityTable;
			break;
		}
	case (GUNMOUNTMESH):
		{
			connectivityTable = gunMountConnectivityTable;
			break;
		}
	default:
		connectivityTable = racerConnectivityTable;
	}

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

	Vertex* vertices = mesh->vertices;
	unsigned long* indices = mesh->indices;
	int vertexCount = mesh->vertexCount;
	int indexCount = mesh->indexCount;

	int numFaces = indexCount / 3;

	unsigned long index0, index1, index2, neighbourTri;


	D3DXVECTOR3* points;
	int numVertices = 0;
	D3DXVECTOR3 newV0, newV1;
	D3DXVECTOR3 v0, v1, v2;
	D3DXVECTOR3 norm;


	shadowVertexBuffer->Lock(0, sizeof(D3DXVECTOR3) * mesh->indexCount * 6, (void**) &points, NULL);
	
	for (int i = 0; i < numFaces; i++)
	{
		index0 = indices[3*i];
		index1 = indices[3*i+1];
		index2 = indices[3*i+2];

		norm = vertices[index0].normal + vertices[index1].normal + vertices[index2].normal;
		norm /= 3.0f;

		// The current face is lit. Must check neighbours now
		if (D3DXVec3Dot(&norm, &light) > 0.0f)
		{
			v0 = vertices[index0].position;
			v1 = vertices[index1].position;
			v2 = vertices[index2].position;
			
			// For each edge, check if neighbour tri is lit. If it's not, add this edge

			// EDGE 0
			neighbourTri = connectivityTable[i][0];
			index0 = indices[3*neighbourTri];
			index1 = indices[3*neighbourTri+1];
			index2 = indices[3*neighbourTri+2];

			norm = vertices[index0].normal + vertices[index1].normal + vertices[index2].normal;
			norm /= 3.0f;
			
			if (D3DXVec3Dot(&norm, &light) < 0.0f)
			{
				// Neighbour is not lit! Add this edge!
				newV0 = v0 - light*200;
				newV1 = v1 - light*200;

				points[numVertices++] = v1;
				points[numVertices++] = v0;
				points[numVertices++] = newV0;
				
				points[numVertices++] = newV0;
				points[numVertices++] = newV1;
				points[numVertices++] = v1;

			}
			
			// EDGE 1
			neighbourTri = connectivityTable[i][1];
			index0 = indices[3*neighbourTri];
			index1 = indices[3*neighbourTri+1];
			index2 = indices[3*neighbourTri+2];

			norm = vertices[index0].normal + vertices[index1].normal + vertices[index2].normal;
			norm /= 3.0f;
			
			if (D3DXVec3Dot(&norm, &light) < 0.0f)
			{
				// Neighbour is not lit! Add this edge!
				newV0 = v1 - light*200;
				newV1 = v2 - light*200;

				points[numVertices++] = v2;
				points[numVertices++] = v1;
				points[numVertices++] = newV0;
				
				points[numVertices++] = newV0;
				points[numVertices++] = newV1;
				points[numVertices++] = v2;
			}

			// EDGE 2
			neighbourTri = connectivityTable[i][2];
			index0 = indices[3*neighbourTri];
			index1 = indices[3*neighbourTri+1];
			index2 = indices[3*neighbourTri+2];

			norm = vertices[index0].normal + vertices[index1].normal + vertices[index2].normal;
			norm /= 3.0f;
			
			if (D3DXVec3Dot(&norm, &light) < 0.0f)
			{
				// Neighbour is not lit! Add this edge!
				newV0 = v2 - light*200;
				newV1 = v0 - light*200;

				points[numVertices++] = v0;
				points[numVertices++] = v2;
				points[numVertices++] = newV0;
				
				points[numVertices++] = newV0;
				points[numVertices++] = newV1;
				points[numVertices++] = v0;
			}
			
		}
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

D3DXMATRIX* Drawable::getTransform()
{
	return &transform;
}