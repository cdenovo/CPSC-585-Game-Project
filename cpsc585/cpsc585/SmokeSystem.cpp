#include "SmokeSystem.h"
#include "Renderer.h"

SmokeSystem* SmokeSystem::system = NULL;

SmokeSystem::SmokeSystem()
{
	rocketSmokeBuffer = NULL;
	explosionSmokeBuffer = NULL;
	smokeList = NULL;
	smokeList = new std::list<SmokeParticle*>();
	smokeList->clear();

	system = this;

	D3DXCreateTextureFromFile(Renderer::device, "textures/smoke1.dds", &rocketSmokeTexture);
	D3DXCreateTextureFromFile(Renderer::device, "textures/smoke2.dds", &explosionSmokeTexture);


	Renderer::device->CreateVertexBuffer(MAX_ROCKET_SMOKE_PARTICLES * sizeof(ParticlePoint),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&rocketSmokeBuffer, NULL);

	Renderer::device->CreateVertexBuffer(MAX_EXPLOSION_SMOKE_PARTICLES * sizeof(ParticlePoint),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&explosionSmokeBuffer, NULL);
}


SmokeSystem::~SmokeSystem()
{
	system = NULL;

	if (smokeList)
	{
		smokeList->clear();
		delete smokeList;
	}
}


void SmokeSystem::update(float seconds)
{
	numRocketSmoke = 0;
	numExplosionSmoke = 0;

	if (!(smokeList->empty()))
	{
		ParticlePoint* rocketSmokeParticles = NULL;
		ParticlePoint* explosionSmokeParticles = NULL;

		rocketSmokeBuffer->Lock(0, MAX_ROCKET_SMOKE_PARTICLES * sizeof(ParticlePoint), (void**) &rocketSmokeParticles, D3DLOCK_DISCARD);
		explosionSmokeBuffer->Lock(0, MAX_EXPLOSION_SMOKE_PARTICLES * sizeof(ParticlePoint), (void**) &explosionSmokeParticles, D3DLOCK_DISCARD);
		
		SmokeParticle* currentParticle = NULL;

		for (std::list<SmokeParticle*>::iterator iter = smokeList->begin(); iter != smokeList->end();)
		{
			currentParticle = (*iter);
			
			if (currentParticle && currentParticle->destroyed)
			{
				delete currentParticle;
				iter = smokeList->erase(iter);
			}
			else if (currentParticle && !(currentParticle->destroyed))
			{
				// Write this out to the vertex buffer for drawing
				if ((currentParticle->typeOfSmoke == ROCKET_SMOKE) && (numRocketSmoke <= MAX_ROCKET_SMOKE_PARTICLES))
				{
					rocketSmokeParticles[numRocketSmoke].pos = currentParticle->position;
					rocketSmokeParticles[numRocketSmoke].color = (D3DCOLOR) currentParticle->color;

					++numRocketSmoke;
				}
				else if (numExplosionSmoke <= MAX_EXPLOSION_SMOKE_PARTICLES)
				{
					explosionSmokeParticles[numExplosionSmoke].pos = currentParticle->position;
					explosionSmokeParticles[numExplosionSmoke].color = (D3DCOLOR) currentParticle->color;
						
					++numExplosionSmoke;
				}

				currentParticle->update(seconds);

				++iter;
			}
		}

		rocketSmokeBuffer->Unlock();
		explosionSmokeBuffer->Unlock();
	}
}


void SmokeSystem::addSmoke(SmokeType type, SmokeParticle* particle)
{
	if (!particle)
		return;

	if (type == ROCKET_SMOKE)
	{
		particle->initialize(Renderer::device, &(hkVector4(0,1.0f,0)), 1.0f, 1.0f, 200, 100, 100, 140, ROCKET_SMOKE);
	}
	else
	{
		particle->initialize(Renderer::device, &(hkVector4(0,-1.0f,0)), 1.0f, 4.0f, 255, 50, 50, 50, EXPLOSION_SMOKE);
	}


	smokeList->push_back(particle);
}


void SmokeSystem::render(SmokeType type)
{
	IDirect3DDevice9* device = Renderer::device;

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

	if (type == ROCKET_SMOKE)
	{
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(1));
		device->SetTexture(0, rocketSmokeTexture);

		device->SetStreamSource(0, rocketSmokeBuffer, 0, sizeof(ParticlePoint));
		device->DrawPrimitive(D3DPT_POINTLIST, 0, numRocketSmoke);
	}
	else
	{
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(30));
		device->SetTexture(0, explosionSmokeTexture);

		device->SetStreamSource(0, explosionSmokeBuffer, 0, sizeof(ParticlePoint));
		device->DrawPrimitive(D3DPT_POINTLIST, 0, numExplosionSmoke);
	}

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
