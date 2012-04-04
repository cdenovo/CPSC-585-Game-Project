#pragma once
#include "drawable.h"

struct ParticlePoint
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
};

class Particle :
	public Drawable
{
public:
	Particle(void);
	~Particle(void);
	void render(IDirect3DDevice9* device);
	void initialize(IDirect3DDevice9* device, int frameCount, std::string* frameList, float size,
		float secondsPerFrame, float gravity, hkVector4* velocity, hkVector4* position, float timeUntilFade);
	void update(float seconds);

private:
	inline DWORD FtoDw(float f)
	{
	    return *((DWORD*)&f);
	}


public:
	hkVector4 velocity;
	hkVector4 position;
	float gravity;
	float fadeTime;
	float secondsPerFrame;
	float size;


private:
	int numFrames;
	bool fading;
	IDirect3DTexture9** frames;
	int currentFrame;

	float currentTime;
	float totalTime;

	IDirect3DVertexBuffer9* vertexBuffer;
};
