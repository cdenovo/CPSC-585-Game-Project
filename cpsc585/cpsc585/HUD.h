#pragma once
#include "Intention.h"
#include "Drawable.h"

enum AbilityType { LASER, SPEED, LEFT, RIGHT };

class HUD
{
public:
	HUD(int width, int height);
	~HUD(void);
	void initialize(IDirect3DDevice9* device);
	void setSelectedAbility(AbilityType ability);
	AbilityType getSelectedAbility();
	void update(Intention intention);
	void render();
	void shutdown();
	void setSpeed(float speed);
	void setHealth(int health);
	void setCheckpointTime(int time);
	void setLap(int lap);

private:
	void showRadial(bool enabled);
	void drawHealth();
	void drawCheckpointTime();
	void drawLap();

	AbilityType selectedAbility;
	ID3DXSprite* sprite;
	IDirect3DTexture9* radialMenuTexture;
	IDirect3DTexture9* reticuleTexture;
	IDirect3DTexture9* speedoTexture;
	IDirect3DTexture9* needleTexture;
	IDirect3DTexture9* numbersTexture;

	RECT* laserRect;
	RECT* speedRect;
	RECT* leftRect;
	RECT* rightRect;
	
	RECT* currentRect;

	D3DXVECTOR3* radialPos;
	D3DXVECTOR3* centre;
	D3DXVECTOR3* speedoPos;

	bool radialEnabled;

	D3DXMATRIX needleTrans;

	float currentSpeed;
	int currentHealth;
	int currentCheckpointTime;
	int currentLap;

	int screenWidth, screenHeight;
};

