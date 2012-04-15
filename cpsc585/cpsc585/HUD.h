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

	bool showAmmo;
	AbilityType ammoIconType;

	int rocketAmmo, landmineAmmo, speedAmmo;

private:
	void showRadial(bool enabled);
	void drawHealth();
	void drawPosition();
	void drawLap();
	void drawAmmo();

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
	IDirect3DTexture9* iconsTexture;

	RECT* laserRect;
	RECT* speedRect;
	RECT* rocketRect;
	RECT* mineRect;

	RECT* rocketIconRect;
	RECT* landmineIconRect;
	RECT* speedIconRect;
	
	RECT* currentRect;

	D3DXVECTOR3* radialPos;
	D3DXVECTOR3* centre;
	D3DXVECTOR3* speedoPos;
	D3DXVECTOR3* ammoPos;

	bool radialEnabled;

	D3DXMATRIX needleTrans;

	float currentSpeed;
	int currentHealth;
	int position;
	int currentLap;
	int numLapsToWin;

	int screenWidth, screenHeight;
};

