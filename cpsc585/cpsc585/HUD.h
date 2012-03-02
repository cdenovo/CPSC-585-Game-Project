#pragma once
#include "Intention.h"
#include "Drawable.h"

enum AbilityType { LASER, SPEED, LEFT, RIGHT };

class HUD
{
public:
	HUD();
	~HUD(void);
	void initialize(IDirect3DDevice9* device);
	void setSelectedAbility(AbilityType ability);
	void update(Intention intention);
	void render();
	void shutdown();

private:
	void showRadial(bool enabled);

	AbilityType selectedAbility;
	ID3DXSprite* sprite;
	IDirect3DTexture9* radialMenuTexture;

	RECT* laserRect;
	RECT* speedRect;
	RECT* leftRect;
	RECT* rightRect;
	
	RECT* currentRect;

	D3DXVECTOR3* radialPos;

	bool radialEnabled;
};

