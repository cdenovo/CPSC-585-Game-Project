#pragma once
#include "Intention.h"
#include "Drawable.h"

enum HUDType { RADIALMENU, SPEEDOMETER };

class HUD
{
public:
	HUD(IDirect3DDevice9* _device, HUDType hudType);
	~HUD(void);
	void setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);	// In Radians
	void setSelectedAbility(int index);
	void update(Intention intention);//, D3DXVECTOR3 cameraPosition);

	Drawable* drawable;
	hkpRigidBody* body;

private:
	int selectedAbilityIndex;
	IDirect3DDevice9* device;
	IDirect3DTexture9* laserTexture;
	IDirect3DTexture9* speedTexture;
	IDirect3DTexture9* leftTexture;
	IDirect3DTexture9* rightTexture;
};

