#pragma once
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>

#include <string>

using namespace std;

#include "Camera.h"
#include "Model.h"


class Renderer
{
public:
	Renderer();
	~Renderer();
	bool initialize(int width, int height, HWND hwnd, float zNear, float zFar);
	void shutdown();
	void render();
	void setText(string* sentences, int count);
	
private:
	void writeText(string text, int line);

	IDirect3D9* d3dObject;
	IDirect3DDevice9* device;

	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;

	Camera* camera;
	Model* model;
	
	ID3DXFont* font;
	
	int numSentences;
	string* sentences;

	int rotY;
};
