#include "HUD.h"


HUD::~HUD(void)
{
}

HUD::HUD(int width, int height)
{
	radialEnabled = false;
	selectedAbility = LASER;
	sprite = NULL;
	radialMenuTexture = NULL;
	reticuleTexture = NULL;

	laserRect = new RECT();
	speedRect = new RECT();
	leftRect = new RECT();
	rightRect = new RECT();

	laserRect->top = 0;
	laserRect->bottom = 300;
	laserRect->right = 300;
	laserRect->left = 0;

	speedRect->top = 0;
	speedRect->bottom = 300;
	speedRect->right = 600;
	speedRect->left = 300;

	leftRect->top = 300;
	leftRect->bottom = 600;
	leftRect->right = 300;
	leftRect->left = 0;

	rightRect->top = 300;
	rightRect->bottom = 600;
	rightRect->right = 600;
	rightRect->left = 300;

	currentRect = laserRect;


	radialPos = new D3DXVECTOR3();
	radialPos->x = 50.0f;
	radialPos->y = 20.0f;
	radialPos->z = 0.0f;

	centre = new D3DXVECTOR3();
	centre->x = width / 2.0f;
	centre->y = height / 2.0f;
	centre->z = 0.0f;
}

void HUD::initialize(IDirect3DDevice9* device)
{
	D3DXCreateTextureFromFile(device, "radialMenu.dds", &radialMenuTexture);
	D3DXCreateTextureFromFile(device, "reticule.dds", &reticuleTexture);
	D3DXCreateSprite(device, &sprite);
}

void HUD::shutdown()
{
	currentRect = NULL;

	if (radialPos)
	{
		delete radialPos;
		radialPos = NULL;
	}

	if (centre)
	{
		delete centre;
		centre = NULL;
	}

	if (laserRect)
	{
		delete laserRect;
		laserRect = NULL;
	}

	if (speedRect)
	{
		delete speedRect;
		speedRect = NULL;
	}

	if (leftRect)
	{
		delete leftRect;
		leftRect = NULL;
	}

	if (rightRect)
	{
		delete rightRect;
		rightRect = NULL;
	}

	if (radialMenuTexture)
	{
		radialMenuTexture->Release();
		radialMenuTexture = NULL;
	}

	if (reticuleTexture)
	{
		reticuleTexture->Release();
		reticuleTexture = NULL;
	}

	if (sprite)
	{
		sprite->Release();
		sprite = NULL;
	}
}


void HUD::showRadial(bool enabled)
{
	radialEnabled = enabled;
}

void HUD::setSelectedAbility(AbilityType ability)
{
	selectedAbility = ability;

	switch (ability)
	{
	case LASER:
		currentRect = laserRect;
		break;
	case SPEED:
		currentRect = speedRect;
		break;
	case LEFT:
		currentRect = leftRect;
		break;
	case RIGHT:
		currentRect = rightRect;
		break;
	default:
		currentRect = laserRect;
	}
}

void HUD::update(Intention intention)//, D3DXVECTOR3 cameraPosition)
{
	// Code for updating the speedometer

	// Code for updating the Radial menu
	if (intention.lbumpPressed)
	{
		radialEnabled = true;

		if(intention.rightStickY > 10000 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // UP
			setSelectedAbility(LASER);
		}
		else if(intention.rightStickY < -10000 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // DOWN
			setSelectedAbility(SPEED);
		}
		else if(intention.rightStickX > 10000 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ // RIGHT
			setSelectedAbility(RIGHT);
		}
		else if(intention.rightStickX < -10000 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ //LEFT
			setSelectedAbility(LEFT);
		}
	}
	else
	{
		radialEnabled = false;
	}
	


	// Code for updating the checkpoint timer

}


void HUD::render()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	// Draw reticule
	sprite->Draw(reticuleTexture, NULL, &(D3DXVECTOR3(16,16,0)), centre, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f));

	// Draw speedometer
	

	// Draw radial menu
	if (radialEnabled)
	{
		sprite->Draw(radialMenuTexture, currentRect, NULL, radialPos, 0xFFFFFFFF);
	}


	// Draw checkpoint timer

	sprite->End();
}


AbilityType HUD::getSelectedAbility()
{
	return selectedAbility;
}
