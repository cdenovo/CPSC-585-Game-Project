#include "Renderer.h"


Renderer::Renderer()
{
	d3dObject = NULL;
	device = NULL;
	
	camera = NULL;
	model = NULL;
	font = NULL;

	sentences = NULL;
	numSentences = 0;
}


Renderer::~Renderer()
{
}

bool Renderer::initialize(int width, int height, HWND hwnd, float zNear, float zFar)
{
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS params;

	ZeroMemory(&params,sizeof(params));

	params.Windowed = TRUE;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	// VSYNC. Change to INTERVAL_IMMEDIATE to turn off VSYNC
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D16;

	
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
	camera->setPos(0.0f, 0.0f, -30.0f);
	
	model = new Model;
	model->initialize(device);
	rotY = 0;





	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light
	
	D3DLIGHT9 light;    // create the light struct
    D3DMATERIAL9 material;    // create the material struct
	
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(1.0f, -0.3f, 1.0f);
	
    device->SetLight(0, &light);    // send the light struct properties to light #0
	device->LightEnable(0, TRUE);    // turn on light #0

	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

	device->SetMaterial(&material);    // set the globably-used material to &material


	// Set up font stuff
	D3DXCreateFont(device, 0, 10, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "", &font);



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

	if (font)
	{
		font->Release();
		font = NULL;
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
	
	device->SetRenderState(D3DRS_ZENABLE, TRUE);

	camera->render();

	// Get view matrix
	camera->getViewMatrix(viewMatrix);

	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(25, 25, 100), 1.0f, 0);
	
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	rotY += 1;
	if (rotY >360)
		rotY = 0;
	device->BeginScene();
	
	model->render(device, 0.0f, 0.0f, 0.0f, 10, rotY, 0);

	model->render(device, -10.0f, -2.0f, 0.0f, 10, rotY, 45);
	
	for (int i = 0; i < numSentences; i++)
	{
		writeText(sentences[i], i);
	}

	device->EndScene();

	device->Present(NULL, NULL, NULL, NULL);

	return;
}

void Renderer::writeText(string text, int line)
{
	device->SetTransform(D3DTS_PROJECTION, &orthoMatrix);

	RECT rect;
	SetRect(&rect, 20, 20 + (line*30), 600, 600);
	
	font->DrawText(NULL, text.c_str(), -1, &rect, DT_LEFT|DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void Renderer::setText(string* sentenceArray, int count)
{
	numSentences = count;
	
	sentences = new string[count];

	for (int i = 0; i < count; i++)
	{
		sentences[i] = string(sentenceArray[i]);
	}
}