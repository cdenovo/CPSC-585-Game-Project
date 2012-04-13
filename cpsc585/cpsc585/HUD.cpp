#include "HUD.h"


HUD::~HUD(void)
{
}

HUD::HUD(int width, int height)
{
	screenWidth = width;
	screenHeight = height;

	radialEnabled = false;
	selectedAbility = LASER;
	sprite = NULL;
	radialMenuTexture = NULL;
	reticuleTexture = NULL;

	laserRect = new RECT();
	mineRect = new RECT();
	rocketRect = new RECT();
	speedRect = new RECT();

	laserRect->top = 0;
	laserRect->bottom = 256;
	laserRect->right = 256;
	laserRect->left = 0;

	mineRect->top = 0;
	mineRect->bottom = 256;
	mineRect->right = 512;
	mineRect->left = 256;

	rocketRect->top = 256;
	rocketRect->bottom = 512;
	rocketRect->right = 256;
	rocketRect->left = 0;

	speedRect->top = 256;
	speedRect->bottom = 512;
	speedRect->right = 512;
	speedRect->left = 256;

	currentRect = laserRect;


	radialPos = new D3DXVECTOR3();
	radialPos->x = 50.0f;
	radialPos->y = 20.0f;
	radialPos->z = 0.0f;

	centre = new D3DXVECTOR3();
	centre->x = width / 2.0f;
	centre->y = height / 2.0f;
	centre->z = 0.0f;


	speedoPos = new D3DXVECTOR3();
	speedoPos->x = width - 150.0f;
	speedoPos->y = height - 150.0f;
	speedoPos->z = 0.0f;

	D3DXMatrixIdentity(&needleTrans);

	currentSpeed = 0;
	currentHealth = 100;
	position = 0;
	currentLap = 0;
	numLapsToWin = 0;
}

void HUD::initialize(IDirect3DDevice9* device)
{
	D3DXCreateTextureFromFile(device, "textures/radialMenu.dds", &radialMenuTexture);
	D3DXCreateTextureFromFile(device, "textures/reticule.dds", &reticuleTexture);
	D3DXCreateTextureFromFile(device, "textures/speedometer.dds", &speedoTexture);
	D3DXCreateTextureFromFile(device, "textures/needle.dds", &needleTexture);
	D3DXCreateTextureFromFile(device, "textures/numbers.dds", &numbersTexture);
	D3DXCreateTextureFromFile(device, "textures/healthBar.dds", &healthBarTexture);
	D3DXCreateTextureFromFile(device, "textures/healthBarBorder.dds", &healthBarBorderTexture);
	D3DXCreateTextureFromFile(device, "textures/lapPositions.dds", &lapPositionsTexture);
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

	if (speedoPos)
	{
		delete speedoPos;
		speedoPos = NULL;
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

	if (mineRect)
	{
		delete mineRect;
		mineRect = NULL;
	}

	if (rocketRect)
	{
		delete rocketRect;
		rocketRect = NULL;
	}

	if (speedRect)
	{
		delete speedRect;
		speedRect = NULL;
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

	if (speedoTexture)
	{
		speedoTexture->Release();
		speedoTexture = NULL;
	}

	if (needleTexture)
	{
		needleTexture->Release();
		needleTexture = NULL;
	}

	if (numbersTexture)
	{
		numbersTexture->Release();
		numbersTexture = NULL;
	}

	if (healthBarTexture)
	{
		healthBarTexture->Release();
		healthBarTexture = NULL;
	}

	if (healthBarBorderTexture)
	{
		healthBarBorderTexture->Release();
		healthBarBorderTexture = NULL;
	}

	if (lapPositionsTexture)
	{
		lapPositionsTexture->Release();
		lapPositionsTexture = NULL;
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
	if (ability == selectedAbility)
		return;

	Sound::sound->playSelect(Sound::sound->playerEmitter);

	selectedAbility = ability;

	switch (ability)
	{
	case LASER:
		currentRect = laserRect;
		break;
	case SPEED:
		currentRect = speedRect;
		break;
	case ROCKET:
		currentRect = rocketRect;
		break;
	case LANDMINE:
		currentRect = mineRect;
		break;
	default:
		currentRect = laserRect;
	}
}

void HUD::update(Intention intention)
{
	// Code for updating the Radial menu
	if (intention.lbumpPressed)
	{
		radialEnabled = true;

		if(intention.rightStickY > 10000 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // UP
			setSelectedAbility(LASER);
		}
		else if(intention.rightStickY < -10000 && intention.rightStickX > -20000 && intention.rightStickX < 20000){ // DOWN
			setSelectedAbility(LANDMINE);
		}
		else if(intention.rightStickX > 10000 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ // RIGHT
			setSelectedAbility(SPEED);
		}
		else if(intention.rightStickX < -10000 && intention.rightStickY < 10000 && intention.rightStickY > -10000){ //LEFT
			setSelectedAbility(ROCKET);
		}
	}
	else
	{
		radialEnabled = false;
	}
	
}


void HUD::render()
{
	D3DXMATRIX origTrans;
	sprite->GetTransform(&origTrans);
	
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	// Draw reticule
	sprite->Draw(reticuleTexture, NULL, &(D3DXVECTOR3(16,16,0)), centre, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f));

	// Draw speedometer
	
	sprite->SetTransform(&needleTrans);
	sprite->Draw(needleTexture, NULL, &(D3DXVECTOR3(120,8,0)), speedoPos,  0xFFFFFFFF);
	sprite->SetTransform(&origTrans);

	sprite->Draw(speedoTexture, NULL, &(D3DXVECTOR3(128,124,0)), speedoPos,  0xFFFFFFFF);
	

	// Draw radial menu
	if (radialEnabled)
	{
		sprite->Draw(radialMenuTexture, currentRect, NULL, radialPos, 0xFFFFFFFF);
	}
	else
	{
		sprite->Draw(radialMenuTexture, currentRect, NULL, radialPos, D3DCOLOR_ARGB(120, 255, 255, 255));
	}


	// Draw position
	drawPosition();

	// Draw health
	drawHealth();

	// Draw lap
	drawLap();


	sprite->End();
}


AbilityType HUD::getSelectedAbility()
{
	return selectedAbility;
}

void HUD::setSpeed(float speed)
{
	if (speed < 0.0f)
		speed *= -1;

	currentSpeed = speed;

	float angle = speed * 0.038f - 0.78f;

	// Code for updating the speedometer
	D3DXMATRIX transToCentre, transToPos, rotate;

	D3DXMatrixTranslation(&transToCentre, -speedoPos->x, -speedoPos->y, 0.0f);
	D3DXMatrixRotationZ(&rotate, angle);
	D3DXMatrixTranslation(&transToPos, speedoPos->x, speedoPos->y, 0.0f);

	D3DXMatrixMultiply(&needleTrans, &transToCentre, &rotate);
	D3DXMatrixMultiply(&needleTrans, &needleTrans, &transToPos);
}

void HUD::setHealth(int health)
{
	currentHealth = health;
}

void HUD::drawHealth()
{
	char healthString[4];
	_itoa_s(currentHealth, healthString, 4, 10);

	D3DXVECTOR3 drawPos;
	drawPos.x = 64.0f;
	drawPos.y = screenHeight - 300.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 actualCenter, currCenter;
	actualCenter.x = 16.0f;
	actualCenter.y = 256.0f;
	actualCenter.z = 0.0f;

	RECT current;
	
	current.top = (LONG) hkMath::ceil(((100 - currentHealth) / 100.0f) * 512.0f);
	current.bottom = 512;
	current.left = 0;
	current.right = 32;

	currCenter.x = 16.0f;
	currCenter.y = 256.0f - current.top;
	currCenter.z = 0.0f;


	sprite->Draw(healthBarTexture, &current, &currCenter, &drawPos, 0xFFFFFFFF);
	sprite->Draw(healthBarBorderTexture, NULL, &actualCenter, &drawPos, 0xFFFFFFFF);
}

void HUD::drawPosition()
{
	D3DXVECTOR3 drawPos;
	drawPos.x = screenWidth - 80.0f;
	drawPos.y = 128.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 currCenter;
	currCenter.x = 32.0f;
	currCenter.y = 32.0f;
	currCenter.z = 0.0f;

	RECT current;

	current.top = 0;
	current.bottom = 64;
	current.left = 0;
	current.right = 64;

	switch (position) {
	case 1:
		{
			current.left += 64;
			current.right += 64;
			break;
		}
	case 2:
		{
			current.left += 128;
			current.right += 128;
			break;
		}
	case 3:
		{
			current.left += 192;
			current.right += 192;
			break;
		}
	case 4:
		{
			current.top += 64;
			current.bottom += 64;
			break;
		}
	case 5:
		{
			current.top += 64;
			current.bottom += 64;
			current.left += 64;
			current.right += 64;
			break;
		}
	case 6:
		{
			current.top += 64;
			current.bottom += 64;
			current.left += 128;
			current.right += 128;
			break;
		}
	case 7:
		{
			current.top += 64;
			current.bottom += 64;
			current.left += 192;
			current.right += 192;
			break;
		}
	case 8:
		{
			current.top += 128;
			current.bottom += 128;
			break;
		}
	default:
		break;
	}
	
	sprite->Draw(lapPositionsTexture, &current, &currCenter, &drawPos, 0xFFFFFFFF);
}


void HUD::setPosition(int pos)
{
	position = pos;
}

void HUD::setLap(int lap, int numToWin)
{
	currentLap = lap;
	numLapsToWin = numToWin;
}


void HUD::drawLap()
{
	char lapString[3];
	
	// This assumes that we will never be doing more than 9 laps
	lapString[0] = (char) currentLap + '0';
	lapString[1] = '/';
	lapString[2] = (char) numLapsToWin + '0';

	D3DXVECTOR3 drawPos;
	drawPos.x = screenWidth - 192.0f;
	drawPos.y = 45.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 currCenter;
	currCenter.x = 16.0f;
	currCenter.y = 32.0f;
	currCenter.z = 0.0f;

	RECT current;

	current.top = 0;
	current.bottom = 64;
	current.left = 0;
	current.right = 64;

	sprite->Draw(lapPositionsTexture, &current, &currCenter, &drawPos, 0xFFFFFFFF);
	drawPos.x += 80.0f;

	current.right = 32;


	for (int i = 0; i < 5; i++)
	{
		if (lapString[i] == '/')
		{
			current.left = 32 * 11;
			current.right = 32 * 12;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos, 0xFFFFFFFF);

			drawPos.x += 32.0f;
		}
		else if ((lapString[i] >= '0') && (lapString[i] <= ':'))
		{
			int num = (int) lapString[i] - '0';

			current.left = 32 * num;
			current.right = 32 * (num + 1);

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos, 0xFFFFFFFF);

			drawPos.x += 32.0f;
		}
		else
		{
			break;
		}
	}
	
}