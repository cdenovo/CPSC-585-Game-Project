#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

class Renderer
{
public:
	Renderer();
	~Renderer();
	bool initialize(int width, int height, HWND hwnd, float zNear, float zFar);
	void shutdown();
	void render();

private:
	ID3D10Device* device;
	IDXGISwapChain* swapChain;
	ID3D10RenderTargetView* renderTargetView;
	ID3D10Texture2D* depthStencilBuffer;
	ID3D10DepthStencilState* depthStencilState;
	ID3D10DepthStencilView* depthStencilView;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;
};

