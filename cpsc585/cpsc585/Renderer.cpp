#include "Renderer.h"


Renderer::Renderer()
{
	device = NULL;
	swapChain = NULL;
	renderTargetView = NULL;
	depthStencilBuffer = NULL;
	depthStencilState = NULL;
	depthStencilView = NULL;
}


Renderer::~Renderer()
{
}

bool Renderer::initialize(int width, int height, HWND hwnd, float zNear, float zFar)
{
	// (referenced a tutorial at http://www.rastertek.com/dx10tut03.html)
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D10Texture2D* backBufferPtr;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_VIEWPORT viewport;
	float fieldOfView, screenAspect;


	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = false;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, 
					       &swapChainDesc, &swapChain, &device)))
		return false;

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	if (FAILED(device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView)))
		return false;

	backBufferPtr->Release();
	backBufferPtr = NULL;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer)))
		return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
		return false;

	device->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView)))
		return false;

	device->OMSetRenderTargets(1, &renderTargetView, depthStencilView);


	// Setup the viewport for rendering.
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	// Create the viewport.
	device->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, zNear, zFar);

	D3DXMatrixIdentity(&worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&orthoMatrix, (float)width, (float)height, zNear, zFar);

	return true;
}

void Renderer::shutdown()
{
	// (referenced a tutorial at http://www.rastertek.com/dx10tut03.html)

	if (swapChain)
		swapChain->SetFullscreenState(false, NULL);

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = NULL;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = NULL;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = NULL;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = NULL;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = NULL;
	}

	if (device)
	{
		device->Release();
		device = NULL;
	}
}

void Renderer::render()
{
	device->ClearRenderTargetView(renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	device->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);



	// Now put stuff to render here




	// Present (syncing with a refresh rate of 60hz)
	swapChain->Present(1, 0);
	

	return;
}
