#include "Mesh.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::loadMesh(std::string filename)
{
	// Set vertexCount and indexCount
	std::ifstream filestream(filename, std::ifstream::binary);
	
	filestream.read((char*)&vertexCount, 4);
	filestream.read((char*)&indexCount, 4);

	vertices = new Vertex[vertexCount];
	indices = new unsigned long[indexCount];

	// Populate vertex buffer and index buffer
	for (int i = 0; i < vertexCount; i++)
	{
		filestream.read((char*)&vertices[i].position.x, 4);
		filestream.read((char*)&vertices[i].position.y, 4);
		filestream.read((char*)&vertices[i].position.z, 4);

		filestream.read((char*)&vertices[i].normal.x, 4);
		filestream.read((char*)&vertices[i].normal.y, 4);
		filestream.read((char*)&vertices[i].normal.z, 4);

		filestream.read((char*)&vertices[i].u, 4);
		filestream.read((char*)&vertices[i].v, 4);
	}

	for (int i = 0; i < indexCount; i++)
	{
		filestream.read((char*)&indices[i], 4);
	}

	filestream.close();
	
	return;
}
