#include "Renderer.h"


Renderer::Renderer()
{
	d3dObject = NULL;
	device = NULL;
	
	camera = NULL;
	model = NULL;
}


Renderer::~Renderer()
{
}

bool Renderer::initialize(int width, int height, HWND hwnd, float zNear, float zFar)
{
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS params;

	ZeroMemory(&params,sizeof(params));

	params.Windowed=TRUE;
	params.SwapEffect=D3DSWAPEFFECT_DISCARD;
	params.BackBufferFormat=D3DFMT_UNKNOWN;
	params.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	
	d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);

	device->SetRenderState(D3DRS_LIGHTING, FALSE);


	D3DVIEWPORT9 viewport;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = zNear;
	viewport.MaxZ = zFar;
	viewport.X = 0;
	viewport.Y = 0;

	device->SetViewport(&viewport);

	float fieldOfView, screenAspect;

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, zNear, zFar);

	D3DXMatrixIdentity(&worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&orthoMatrix, (float)width, (float)height, zNear, zFar);



	camera = new Camera;
	camera->setPos(0.0f, 0.0f, -10.0f);
	
	model = new Model;
	model->initialize(device);

	return true;
}

void Renderer::shutdown()
{
	if (model)
	{
		model->shutdown();
		delete model;
		model = NULL;
	}

	if (camera)
	{
		delete camera;
		camera = NULL;
	}

	if (d3dObject)
	{
		d3dObject->Release();
		d3dObject = NULL;
	}

	if (device)
	{
		device->Release();
		device = NULL;
	}
}

void Renderer::render()
{
	D3DXMATRIX viewMatrix;

	camera->render();

	// Get view matrix
	camera->getViewMatrix(viewMatrix);

	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(25, 25, 100), 1.0f, 0);

	device->BeginScene();

	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	model->render(device);

	device->EndScene();

	device->Present(NULL, NULL, NULL, NULL);

	return;
}
