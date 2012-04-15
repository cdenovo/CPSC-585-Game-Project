#include "SmokeParticle.h"


SmokeParticle::SmokeParticle()
{
	velocity.set(0,0,0);
	position = D3DXVECTOR3(0,0,0);
	timeUntilFadeStart = 0.0f;
	fadingTime = 0.0f;
	fadingTimer = 0.0f;
	currentTime = 0.0f;
	totalTime = 0.0f;
	alpha = 0;
	r = 0;
	g = 0;
	b = 0;

	color = D3DCOLOR_ARGB(alpha, r, g, b);

	fading = false;
	destroyed = false;
	typeOfSmoke = ROCKET_SMOKE;
}


SmokeParticle::~SmokeParticle()
{

}

void SmokeParticle::initialize(IDirect3DDevice9* device, hkVector4* vel, float timeUntilFade, float fadingTimePeriod, int a,
	int red, int green, int blue, SmokeType type)
{
	typeOfSmoke = type;
	velocity.setXYZ(*vel);
	timeUntilFadeStart = timeUntilFade;
	fadingTime = fadingTimePeriod;
	fadingTimer = fadingTimePeriod;

	totalTime = timeUntilFadeStart + fadingTime;
	fading = false;
	currentTime = 0.0f;

	alpha = a;
	r = red;
	g = green;
	b = blue;

	color = D3DCOLOR_ARGB(alpha, r, g, b);
}


void SmokeParticle::update(float seconds)
{
	currentTime += seconds;

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

		color = D3DCOLOR_ARGB((int) hkMath::floor(alpha * fadingTimer / fadingTime), r, g, b);
	}
	else if (currentTime >= totalTime)
	{
		destroyed = true;
	}
	
	position.x += velocity(0) * seconds;
	position.y += velocity(1) * seconds;
	position.z += velocity(2) * seconds;
}


void SmokeParticle::setPosition(hkVector4* pos)
{
	position.x = (*pos)(0);
	position.y = (*pos)(1);
	position.z = (*pos)(2);
}
