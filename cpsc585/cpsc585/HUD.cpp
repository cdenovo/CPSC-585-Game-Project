#include "HUD.h"


HUD::~HUD(void)
{
}

HUD::HUD(IDirect3DDevice9* _device, HUDType hudType)
{
	device = _device;
	selectedAbilityIndex = 1;

	laserTexture = NULL; speedTexture = NULL; leftTexture = NULL; rightTexture = NULL;
	std::string laser = "LaserSelected.dds";
	std::string speed = "SpeedSelected.dds";
	std::string left = "LeftSelected.dds";
	std::string right = "RightSelected.dds";
	D3DXCreateTextureFromFile(device, laser.c_str(), &laserTexture);
	D3DXCreateTextureFromFile(device, speed.c_str(), &speedTexture);
	D3DXCreateTextureFromFile(device, left.c_str(), &leftTexture);
	D3DXCreateTextureFromFile(device, right.c_str(), &rightTexture);

	
	switch (hudType)
	{
	case RADIALMENU:
		drawable = new Drawable(RACER, "LaserSelected.dds", device);
		break;
	case SPEEDOMETER:
		drawable = new Drawable(RACER, "LaserSelected.dds", device);
		break;
	default:
		drawable = new Drawable(RACER, "LaserSelected.dds", device);
	}
	
}

void HUD::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotZ));
}

void HUD::setSelectedAbility(int index)
{
	selectedAbilityIndex = index;
}

void HUD::update(Intention intention)//, D3DXVECTOR3 cameraPosition)
{
	//D3DXVECTOR3 camPos = cameraPosition;
	//setPosAndRot(camPos.x, camPos.y, camPos.z+5.0f, 0.0f, 3*3.141592f, 2*3.141592f);

	// Code for updating the speedometer

	// Code for updating the Radial menu
	if(intention.rightStickY > 0 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // UP
		drawable->changeTexture(laserTexture);
	}

	else if(intention.rightStickY < 0 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // DOWN
		drawable->changeTexture(speedTexture);
	}

	else if(intention.rightStickX > 0 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ // RIGHT
		drawable->changeTexture(rightTexture);
	}

	else if(intention.rightStickX < 0 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ //LEFT
		drawable->changeTexture(leftTexture);
	}


	// Code for updating the checkpoint timer

}
