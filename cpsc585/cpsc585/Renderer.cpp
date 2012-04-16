#include "Renderer.h"

Renderer* Renderer::renderer = NULL;
IDirect3DDevice9* Renderer::device = NULL;
D3DXVECTOR3 Renderer::lightDir = D3DXVECTOR3(0,-0.7f,-1);

Renderer::Renderer()
{
	d3dObject = NULL;
	device = NULL;
	
	camera = NULL;
	font = NULL;

	sentences = NULL;
	numSentences = 0;
	numDrawables = 0;
	drawables = NULL;
	currentDrawable = 0;
	hud = NULL;

	shadowQuadVertexBuffer = NULL;

	renderer = this;
}


Renderer::~Renderer()
{
}

bool Renderer::initialize(int width, int height, HWND hwnd, float zNear, float zFar, int numToDraw, char* msg)
{
	numDrawables = numToDraw;
	useTwoSidedStencils = false;

	drawables = new Drawable*[numToDraw];
	dynamicDrawables = new std::vector<Drawable*>();
	dynamicDrawables->clear();
	dynamicDrawables->reserve(100);
	
	hud = new HUD(width, height);

	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DCAPS9 caps;
	d3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if (!(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT))
	{
		LPCTSTR descr = "This program requires a video card that supports hardware vertex processing!";
		strcpy_s((char*) msg, 128, descr);
		
		return false;
	}


	useTwoSidedStencils = ((caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED) != 0);


	D3DPRESENT_PARAMETERS params;

	ZeroMemory(&params,sizeof(D3DPRESENT_PARAMETERS));

	params.Windowed = TRUE;
	params.BackBufferCount= 1;
	params.BackBufferFormat = D3DFMT_X8R8G8B8;
	
	params.BackBufferWidth = width;
	params.BackBufferHeight = height;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hwnd;
	params.EnableAutoDepthStencil = TRUE;
	params.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	// VSYNC. Change to INTERVAL_IMMEDIATE to turn off VSYNC, change to INTERVAL_ONE to turn on VSYNC
	params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// Now need to set up the depth stencil format.
	D3DFORMAT formats[] = { D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24FS8 };
	
	D3DFORMAT format = (D3DFORMAT) 0;
	
	for (int i = 0; i < 3; i++)
	{
		HRESULT result = d3dObject->CheckDeviceFormat(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE, formats[i]);

		if (result == D3D_OK)
		{
			result = d3dObject->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, formats[i]);

			if (result == D3D_OK)
			{
				format = formats[i];
				break;
			}
		}
	}
	
	if (format == (D3DFORMAT) 0)
	{
		LPCTSTR descr = "Couldn't find a suitable depth format!";
		strcpy_s((char*) msg, 128, descr);
		
		return false;
	}
	
	params.AutoDepthStencilFormat = format;

	HRESULT result = d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);

	if (FAILED(result))
	{
		LPCTSTR descr = DXGetErrorDescription(result);
		strcpy_s((char*) msg, 128, descr);
		
		return false;
	}

	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	D3DVIEWPORT9 viewport;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	viewport.X = 0;
	viewport.Y = 0;

	device->SetViewport(&viewport);

	float fieldOfView, screenAspect;

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 2.5f;
	screenAspect = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, zNear, zFar);

	D3DXMatrixIdentity(&worldMatrix);
	
	camera = new Camera;
	
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	
	
	D3DLIGHT9 light;    // create the light struct
	D3DMATERIAL9 material;    // create the material struct
	
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.7f, 1.0f);    // set the light's color
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	D3DXVec3Normalize(&lightDir, &lightDir);
	light.Direction = lightDir;
	
    device->SetLight(0, &light);    // send the light struct properties to light #0
	device->LightEnable(0, TRUE);    // turn on light #0

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	device->SetMaterial(&material);    // set the globably-used material to &material

	// Set up font stuff
	D3DXCreateFont(device, 0, 10, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Terminal", &font);

	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	// Just to be safe (ignored)
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	// Stuff for particle effects
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));
	device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.1f));
	device->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(1280.0f));
	
	// Set fog
	float startFog = 1.0f;
	float endFog = 500.0f;
	device->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(200,120,120,140));
	device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&startFog));
    device->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&endFog));

	// Set up HUD
	hud->initialize(device);

	// Set up Skybox
	skybox = new Skybox(device);

	ShadowPoint* points;

	// Set up shadow texture
	device->CreateVertexBuffer(sizeof(ShadowPoint) * 4, D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED, &shadowQuadVertexBuffer, NULL);

	shadowQuadVertexBuffer->Lock(0, sizeof(ShadowPoint) * 4, (void**) &points, NULL);

	points[0].position = D3DXVECTOR4(0, (FLOAT) height, 0, 1);
	points[1].position = D3DXVECTOR4(0, 0, 0, 1);
	points[2].position = D3DXVECTOR4((FLOAT) width, (FLOAT) height, 0, 1);
	points[3].position = D3DXVECTOR4((FLOAT) width, 0, 0, 1);
	points[0].color = D3DCOLOR_ARGB(180,0,0,0);
	points[1].color = D3DCOLOR_ARGB(180,0,0,0);
	points[2].color = D3DCOLOR_ARGB(180,0,0,0);
	points[3].color = D3DCOLOR_ARGB(180,0,0,0);

	shadowQuadVertexBuffer->Unlock();

	smokeSystem = new SmokeSystem();
	laserSystem = new LaserSystem();

	return true;
}

void Renderer::shutdown()
{
	if (drawables)
	{
		// Clean up drawables
	}

	if (dynamicDrawables)
	{
		dynamicDrawables->clear();
		delete dynamicDrawables;
		dynamicDrawables = NULL;
	}

	if (smokeSystem)
	{
		delete smokeSystem;
		smokeSystem = NULL;
	}

	if (hud)
	{
		// Clean up HUD
		hud->shutdown();
		hud = NULL;
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
	
	// Draw skybox

	camera->updateForSkybox();

	// Get view matrix
	camera->getViewMatrix(viewMatrix);

	// Build shadow volumes for all racers
	for (int i = 0; i < currentDrawable; i++)
	{
		if ((drawables[i]->meshType == RACER) || (drawables[i]->meshType == REARWHEEL)
			|| (drawables[i]->meshType == FRONTWHEEL) || (drawables[i]->meshType == GUNMOUNTMESH))
		{
			drawables[i]->buildShadowVolume(lightDir);
		}
	}
	
	
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, NULL, 1.0f, 0);
	
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	device->BeginScene();

	skybox->render(device);

	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);


	// Now draw rest of the scene

	camera->update();



	// Get view matrix
	camera->getViewMatrix(viewMatrix);

	device->SetTransform(D3DTS_VIEW, &viewMatrix);
	

	device->SetRenderState(D3DRS_FOGENABLE, TRUE);

	for (int i = 0; i < currentDrawable; i++)
	{
		drawables[i]->render(device);
	}

	// Draw dynamic objects that will be removed after this frame (like rockets, lasers, landmines)
	if (!(dynamicDrawables->empty()))
	{
		for (std::vector<Drawable*>::iterator iter = dynamicDrawables->begin();
			iter < dynamicDrawables->end(); ++iter)
		{
			(*iter)->render(device);
		}

		dynamicDrawables->clear();
	}
	
	
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	// Draw stencil shadows
	drawShadows();
	


	// Render SmokeSystem particles
	device->SetTransform(D3DTS_WORLD, &worldMatrix);
	smokeSystem->render(EXPLOSION_SMOKE);
	smokeSystem->render(ROCKET_SMOKE);

	// Render LaserSystem particles (beginning and end points of shots)
	laserSystem->render();


	for (int i = 0; i < numSentences; i++)
	{
		writeText(sentences[i], i);
	}
	
	device->EndScene();


	// Now draw HUD
	hud->render();

	device->Present(NULL, NULL, NULL, NULL);

	return;
}

void Renderer::writeText(std::string text, int line)
{
	RECT rect;
	SetRect(&rect, 20, 20 + (line*30), 600, 600);
	
	font->DrawText(NULL, text.c_str(), -1, &rect, DT_LEFT|DT_NOCLIP, D3DCOLOR_XRGB(200, 50, 50));
}

void Renderer::setText(std::string* sentenceArray, int count)
{
	if ((sentences) && (numSentences < count))
	{
		delete [] sentences;
		sentences = NULL;
	}
	
	numSentences = count;

	sentences = new std::string[count];

	for (int i = 0; i < count; i++)
	{
		sentences[i] = std::string(sentenceArray[i]);
	}
}


int Renderer::addDrawable(Drawable* drawable)
{
	if (currentDrawable < numDrawables)
	{
		drawables[currentDrawable] = drawable;

		if (currentDrawable == 0)
		{
			camera->setFocus(drawable);
		}

		currentDrawable++;

		return (currentDrawable - 1);
	}

	return -1;
}

void Renderer::setFocus(int drawableIndex)
{
	if ((drawableIndex < 0) || (drawableIndex >= currentDrawable))
	{
		return;
	}

	camera->setFocus(drawables[drawableIndex]);
}

IDirect3DDevice9* Renderer::getDevice()
{
	return device;
}

HUD* Renderer::getHUD()
{
	return hud;
}

Camera* Renderer::getCamera()
{
	return camera;
}


// Adds a drawable that will be drawn for only one frame
void Renderer::addDynamicDrawable(Drawable* drawable)
{
	if (!drawable)
		return;

	dynamicDrawables->push_back(drawable);
}

void Renderer::drawShadows()
{
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	// Disable lighting
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Disable writing to depth-buffer
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	
	// Got most of this (and the addEdge() function) from some
	// sample source code online, which said it got most of
	// the code in turn from the DirectX SDK
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

    // If z-test passes, inc/decrement stencil buffer value
    device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

    // Make sure that no pixels get drawn to the frame buffer
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	// Two-sided stencil functionality makes shadows faster
	// (one pass instead of two)
	if (useTwoSidedStencils)
	{
		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
		device->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Now render in only one pass!
		for (int i = 0; i < currentDrawable; i++)
		{
			if ((drawables[i]->meshType == RACER) || (drawables[i]->meshType == REARWHEEL)
				|| (drawables[i]->meshType == FRONTWHEEL) || (drawables[i]->meshType == GUNMOUNTMESH))
			{
				drawables[i]->renderShadowVolume(device);
			}
		}

		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
	}
	else	// Do two passes for shadows if two-sided stencil unavailable
	{
		// Draw front-side of shadow volume in stencil/z only
		for (int i = 0; i < currentDrawable; i++)
		{
			if ((drawables[i]->meshType == RACER) || (drawables[i]->meshType == REARWHEEL)
				|| (drawables[i]->meshType == FRONTWHEEL) || (drawables[i]->meshType == GUNMOUNTMESH))
			{
				drawables[i]->renderShadowVolume(device);
			}
		}

		// Now reverse cull order so back sides of shadow volume are written.
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// Decrement stencil buffer value
		device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);

		// Draw back-side of shadow volume in stencil/z only
		for (int i = 0; i < currentDrawable; i++)
		{
			if ((drawables[i]->meshType == RACER) || (drawables[i]->meshType == REARWHEEL)
				|| (drawables[i]->meshType == FRONTWHEEL) || (drawables[i]->meshType == GUNMOUNTMESH))
			{
				drawables[i]->renderShadowVolume(device);
			}
		}
	}

	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);



    device->SetRenderState(D3DRS_ZENABLE, FALSE);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


	// Only write where stencil value >= 1 (count indicates # of shadows that
    // overlap that pixel)
    device->SetRenderState(D3DRS_STENCILREF, 1);
	device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
    device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// Draw big dark square
	device->SetStreamSource(0, shadowQuadVertexBuffer, 0, sizeof(ShadowPoint));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
