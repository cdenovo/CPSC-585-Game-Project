#include "LaserSystem.h"
#include "Renderer.h"

LaserSystem* LaserSystem::system = NULL;

LaserSystem::LaserSystem()
{
	ballLaserBuffer = NULL;
	fireLaserBuffer = NULL;
	beamLaserBuffer = NULL;

	laserParticleList = NULL;
	laserParticleList = new std::list<LaserParticle*>();
	laserParticleList->clear();

	laserBeamList = NULL;
	laserBeamList = new std::list<LaserBeam*>();
	laserBeamList->clear();

	system = this;

	D3DXCreateTextureFromFile(Renderer::device, "textures/laserBall.dds", &ballLaserTexture);
	D3DXCreateTextureFromFile(Renderer::device, "textures/laserFire.dds", &fireLaserTexture);
	D3DXCreateTextureFromFile(Renderer::device, "textures/laser.dds", &beamLaserTexture);


	Renderer::device->CreateVertexBuffer(MAX_LASER_BALL_PARTICLES * sizeof(ParticlePoint),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&ballLaserBuffer, NULL);

	Renderer::device->CreateVertexBuffer(MAX_LASER_FIRE_PARTICLES * sizeof(ParticlePoint),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&fireLaserBuffer, NULL);

	Renderer::device->CreateVertexBuffer(MAX_LASER_BEAM_PARTICLES * NUM_VERTICES_PER_BEAM * sizeof(Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
		&beamLaserBuffer, NULL);
}


LaserSystem::~LaserSystem()
{
	system = NULL;

	if (laserBeamList)
	{
		laserBeamList->clear();
		delete laserBeamList;
	}

	if (laserParticleList)
	{
		laserParticleList->clear();
		delete laserParticleList;
	}
}


void LaserSystem::update(float seconds)
{
	numBallLaser = 0;
	numFireLaser = 0;
	numBeamLaser = 0;

	// Do particles
	if (!(laserParticleList->empty()))
	{
		ParticlePoint* ballLaserParticles = NULL;
		ParticlePoint* fireLaserParticles = NULL;

		ballLaserBuffer->Lock(0, MAX_LASER_BALL_PARTICLES * sizeof(ParticlePoint), (void**) &ballLaserParticles, D3DLOCK_DISCARD);
		fireLaserBuffer->Lock(0, MAX_LASER_FIRE_PARTICLES * sizeof(ParticlePoint), (void**) &fireLaserParticles, D3DLOCK_DISCARD);
		
		LaserParticle* currentParticle = NULL;

		for (std::list<LaserParticle*>::iterator iter = laserParticleList->begin(); iter != laserParticleList->end();)
		{
			currentParticle = (*iter);
			
			if (currentParticle && currentParticle->destroyed)
			{
				delete currentParticle;
				iter = laserParticleList->erase(iter);
			}
			else if (currentParticle && !(currentParticle->destroyed))
			{
				// Write this out to the vertex buffer for drawing
				if ((currentParticle->typeOfLaser == LASER_BALL) && (numBallLaser <= MAX_LASER_BALL_PARTICLES))
				{
					ballLaserParticles[numBallLaser].pos = currentParticle->position;

					++numBallLaser;
				}
				else if (numFireLaser <= MAX_LASER_FIRE_PARTICLES)
				{
					fireLaserParticles[numFireLaser].pos = currentParticle->position;
						
					++numFireLaser;
				}

				currentParticle->update(seconds);

				++iter;
			}
		}

		ballLaserBuffer->Unlock();
		fireLaserBuffer->Unlock();
	}


	// Do beams
	if (!(laserBeamList->empty()))
	{
		Vertex* beamLaserVertices = NULL;

		beamLaserBuffer->Lock(0, MAX_LASER_BEAM_PARTICLES * NUM_VERTICES_PER_BEAM * sizeof(Vertex),
			(void**) &beamLaserVertices, D3DLOCK_DISCARD);
		
		LaserBeam* currentBeam = NULL;

		for (std::list<LaserBeam*>::iterator iter = laserBeamList->begin(); iter != laserBeamList->end();)
		{
			currentBeam = (*iter);
			
			if (currentBeam && currentBeam->destroyed)
			{
				delete currentBeam;
				iter = laserBeamList->erase(iter);
			}
			else if (currentBeam && !(currentBeam->destroyed) && (numBeamLaser <= MAX_LASER_BEAM_PARTICLES))
			{
				// Write this out to the vertex buffer for drawing
				currentBeam->writeVertices(&beamLaserVertices[numBeamLaser * NUM_VERTICES_PER_BEAM]);
						
				++numBeamLaser;

				currentBeam->update(seconds);

				++iter;
			}
		}

		beamLaserBuffer->Unlock();
	}
}


void LaserSystem::addLaser(hkVector4* startPoint, hkVector4* endPoint)
{
	// Add a beam
	D3DXVECTOR3 start;
	start.x = (*startPoint)(0);
	start.y = (*startPoint)(1);
	start.z = (*startPoint)(2);

	D3DXVECTOR3 end;
	end.x = (*endPoint)(0);
	end.y = (*endPoint)(1);
	end.z = (*endPoint)(2);

	LaserBeam* beam = new LaserBeam(start, end);

	laserBeamList->push_back(beam);

	hkVector4 vel;
	vel.setXYZ(*endPoint);
	vel.sub(*startPoint);

	float distance = vel.length3();
	vel.div(hkVector4(distance, distance, distance));

	vel.mul(LaserBeam::vanishingSpeed);

	LaserParticle* particle = new LaserParticle();
	particle->initialize(Renderer::device, &(hkVector4(0,0,0)), LASER_FIRE, 0.2f);
	particle->setPosition(startPoint);
	laserParticleList->push_back(particle);

	particle = new LaserParticle();
	particle->initialize(Renderer::device, &(hkVector4(0,0,0)), LASER_FIRE, distance / LaserBeam::vanishingSpeed);
	particle->setPosition(endPoint);
	laserParticleList->push_back(particle);

	particle = new LaserParticle();
	particle->initialize(Renderer::device, &vel, LASER_BALL, distance / LaserBeam::vanishingSpeed);
	particle->setPosition(startPoint);
	laserParticleList->push_back(particle);
}


void LaserSystem::render()
{

	IDirect3DDevice9* device = Renderer::device;

	// Render beams
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


	if (numBeamLaser > 0)
	{
		device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
		device->SetTexture(0, beamLaserTexture);

		device->SetStreamSource(0, beamLaserBuffer, 0, sizeof(Vertex));
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numBeamLaser * NUM_VERTICES_PER_BEAM / 3);
	}


	// Render particles
	device->SetRenderState(D3DRS_ALPHAREF, (unsigned long) 50);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	device->SetFVF(D3DFVF_XYZ);

	if (numFireLaser > 0)
	{
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.5f));
		device->SetTexture(0, fireLaserTexture);

		device->SetStreamSource(0, fireLaserBuffer, 0, sizeof(ParticlePoint));
		device->DrawPrimitive(D3DPT_POINTLIST, 0, numFireLaser);
	}

	if (numBallLaser > 0)
	{
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(1));
		device->SetTexture(0, ballLaserTexture);
	
		device->SetStreamSource(0, ballLaserBuffer, 0, sizeof(ParticlePoint));
		device->DrawPrimitive(D3DPT_POINTLIST, 0, numBallLaser);
	}

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
