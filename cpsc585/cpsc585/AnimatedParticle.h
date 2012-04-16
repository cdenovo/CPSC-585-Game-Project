#pragma once

#include "Drawable.h"
#include "DynamicObjManager.h"

enum AnimatedType { ANIM_EXPLOSION, ANIM_SMOKE };

class AnimatedParticle : public Drawable, public DynamicObj
{
public:
	AnimatedParticle();
	~AnimatedParticle();
	void render(IDirect3DDevice9* device);
	void update(float seconds);
	void initialize(IDirect3DDevice9* device, float timeUntilFade, float fadingTimePeriod, AnimatedType type,
		float timePerFrame);

	inline DWORD FtoDw(float f)
	{
	    return *((DWORD*)&f);
	}

	float timeUntilFadeStart;
	float fadingTime;
	float fadingTimer;
	float totalTime;

	AnimatedType typeOfAnim;

private:
	bool fading;
	float currentTime;
	int alpha;
	int baseAlpha;

	int currentFrame;

	static IDirect3DTexture9** explosionTexArray;
	static IDirect3DTexture9** smokeTexArray;

	float timePerFrame;

	IDirect3DVertexBuffer9* particleBuffer;
};
