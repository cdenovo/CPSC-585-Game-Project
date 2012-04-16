#include "AnimatedParticle.h"

IDirect3DTexture9** AnimatedParticle::explosionTexArray = NULL;
IDirect3DTexture9** AnimatedParticle::smokeTexArray = NULL;

AnimatedParticle::AnimatedParticle()
{
	particleBuffer = NULL;

	if (!explosionTexArray)
	{
		explosionTexArray = new IDirect3DTexture9*[5];

		D3DXCreateTextureFromFile(Renderer::device, "textures/fire1.dds", &(explosionTexArray[0]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/fire2.dds", &(explosionTexArray[1]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/fire3.dds", &(explosionTexArray[2]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/fire4.dds", &(explosionTexArray[3]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/fire5.dds", &(explosionTexArray[4]));
	}

	if (!smokeTexArray)
	{
		smokeTexArray = new IDirect3DTexture9*[5];

		D3DXCreateTextureFromFile(Renderer::device, "textures/animSmoke1.dds", &(smokeTexArray[0]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/animSmoke2.dds", &(smokeTexArray[1]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/animSmoke3.dds", &(smokeTexArray[2]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/animSmoke4.dds", &(smokeTexArray[3]));
		D3DXCreateTextureFromFile(Renderer::device, "textures/animSmoke5.dds", &(smokeTexArray[4]));
	}

	Renderer::device->CreateVertexBuffer(sizeof(ParticlePoint),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&particleBuffer, NULL);

	currentFrame = 0;
	timeUntilFadeStart = 0.0f;
	fading = false;
	fadingTime = 0.0f;
	fadingTimer = 0.0f;
	totalTime = 0.0f;
	currentTime = 0.0f;
	timePerFrame = 0.1f;
	destroyed = false;
	baseAlpha = 255;
	drawable = this;
}


AnimatedParticle::~AnimatedParticle()
{
}

void AnimatedParticle::render(IDirect3DDevice9* device)
{
	IDirect3DTexture9** texArray = NULL;
	device->SetTransform(D3DTS_WORLD, &transform);

	if (typeOfAnim == ANIM_EXPLOSION)
	{
		texArray = explosionTexArray;
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(10));
	}
	else
	{
		texArray = smokeTexArray;
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(12));
	}


	ParticlePoint* point;

	particleBuffer->Lock(0, sizeof(ParticlePoint), (void**) &point, D3DLOCK_DISCARD);

	point[0].pos = D3DXVECTOR3(0,0,0);
	point[0].color = D3DCOLOR_ARGB(alpha,255,255,255);

	particleBuffer->Unlock();

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

	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	device->SetTexture(0, texArray[currentFrame]);

	device->SetStreamSource(0, particleBuffer, 0, sizeof(ParticlePoint));
	device->DrawPrimitive(D3DPT_POINTLIST, 0, 1);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


void AnimatedParticle::update(float seconds)
{
	currentTime += seconds;
	currentFrame = ((int) hkMath::floor(currentTime / timePerFrame)) % 5;

	if (!fading && currentTime >= timeUntilFadeStart)
	{
		fading = true;
	}
	else if (fading)
	{
		fadingTimer -= seconds;
		
		if (fadingTimer <= 0.0f)
		{
			destroyed = true;
			fadingTimer = 0.0f;
		}

		alpha = (int) hkMath::floor((float)baseAlpha * fadingTimer / fadingTime);
	}
	else if (currentTime >= totalTime)
	{
		destroyed = true;
	}
}

void AnimatedParticle::initialize(IDirect3DDevice9* device, float timeUntilFade,
	float fadingTimePeriod,	AnimatedType type, float t)
{
	timePerFrame = t;
	timeUntilFadeStart = timeUntilFade;
	fadingTime = fadingTimePeriod;
	fadingTimer = fadingTime;
	totalTime = timeUntilFade + fadingTime;

	typeOfAnim = type;

	if (type == ANIM_EXPLOSION)
	{
		baseAlpha = 255;
	}
	else
	{
		baseAlpha = 150;
	}
}
