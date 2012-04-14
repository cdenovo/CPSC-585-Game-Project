#pragma once
#include "Intention.h"
#include "Drawable.h"
#include "Sound.h"

enum AbilityType { LASER, SPEED, LANDMINE, ROCKET };

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
	void setPosition(int placement);
	void setLap(int lap, int numToWin);

	bool showOne;
	bool showTwo;
	bool showThree;

private:
	void showRadial(bool enabled);
	void drawHealth();
	void drawPosition();
	void drawLap();

	AbilityType selectedAbility;
	ID3DXSprite* sprite;
	IDirect3DTexture9* radialMenuTexture;
	IDirect3DTexture9* reticuleTexture;
	IDirect3DTexture9* speedoTexture;
	IDirect3DTexture9* needleTexture;
	IDirect3DTexture9* numbersTexture;
	IDirect3DTexture9* healthBarTexture;
	IDirect3DTexture9* healthBarBorderTexture;
	IDirect3DTexture9* lapPositionsTexture;
	IDirect3DTexture9* countdownTexture;

	RECT* laserRect;
	RECT* speedRect;
	RECT* rocketRect;
	RECT* mineRect;
	
	RECT* currentRect;

	D3DXVECTOR3* radialPos;
	D3DXVECTOR3* centre;
	D3DXVECTOR3* speedoPos;

	bool radialEnabled;

	D3DXMATRIX needleTrans;

	float currentSpeed;
	int currentHealth;
	int position;
	int currentLap;
	int numLapsToWin;

	int screenWidth, screenHeight;
};

