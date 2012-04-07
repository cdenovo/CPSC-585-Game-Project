#include "Particle.h"
//IDirect3DVertexBuffer9* Particle::vertexBuffer = NULL;

Particle::Particle(void)
{
	vertexBuffer = NULL;

	velocity.set(0,0,0);
	gravity = 0.0f;
	fadeTime = 2.0f;
	secondsPerFrame = 0.0f;
	currentTime = 0.0f;
	totalTime = 0.0f;

	numFrames = 0;
	fading = false;
	frames = NULL;
	currentFrame = 0;
	size = 1.0f;

	D3DXMatrixIdentity(&transform);
}


Particle::~Particle(void)
{
	if (frames)
	{
		for (int i = 0; i < numFrames; i++)
		{
			if (frames[i])
			{
				frames[i]->Release();
				frames[i] = NULL;
			}
		}

		delete [] frames;
	}
}


void Particle::render(IDirect3DDevice9* device)
{
	device->SetRenderState(D3DRS_POINTSIZE, FtoDw(size));
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	device->SetRenderState(D3DRS_ALPHAREF, (unsigned long) 50);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// Apply transforms, THEN call render on the mesh
	D3DXMatrixTranslation(&transform, position(0), position(1), position(2));
	device->SetTransform(D3DTS_WORLD, &transform);
	device->SetTexture(0, frames[currentFrame]);

	
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(ParticlePoint));
	device->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
}

void Particle::initialize(IDirect3DDevice9* device, int frameCount, std::string* frameList, float sz,
	float secPerFrame, float grav, hkVector4* vel, hkVector4* pos, float timeUntilFade)
{
	size = sz;
	secondsPerFrame = secPerFrame;
	gravity = grav;
	velocity.setXYZ(*vel);
	fadeTime = timeUntilFade;
	fading = false;
	totalTime = secondsPerFrame * numFrames;
	currentTime = 0.0f;
	position.setXYZ(*pos);

	numFrames = frameCount;

	frames = new IDirect3DTexture9*[numFrames];

	for (int i = 0; i < frameCount; i++)
	{
		D3DXCreateTextureFromFile(device, frameList[i].c_str(), &frames[i]);
	}


	if (!vertexBuffer)
	{
		device->CreateVertexBuffer(sizeof(ParticlePoint), D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, D3DFVF_XYZ | D3DFVF_DIFFUSE,
			D3DPOOL_MANAGED, &vertexBuffer, NULL);

		ParticlePoint particlePoints[1];
		particlePoints[0].pos = D3DXVECTOR3(0,0,0);
		particlePoints[0].color = D3DCOLOR_ARGB(200, 100, 100, 140);

		ParticlePoint* points;

		vertexBuffer->Lock(0, sizeof(ParticlePoint), (void**) &points, NULL);

		memcpy(points, particlePoints, sizeof(ParticlePoint));

		vertexBuffer->Unlock();
	}
}


void Particle::update(float seconds)
{
	currentTime += seconds;
	
	currentFrame = (int) hkMath::floor(currentTime / secondsPerFrame);
	currentFrame %= numFrames;

	if (!fading && currentTime >= fadeTime)
	{
		fading = true;
		fadeTime = 1.0f;
	}
	else if (fading)
	{
		fadeTime -= seconds;
	}

	position(1) += gravity * seconds;

	hkVector4 movement;
	movement.setXYZ(velocity);
	movement.mul(seconds);

	position.add(movement);
}
