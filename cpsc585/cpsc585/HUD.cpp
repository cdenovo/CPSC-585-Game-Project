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


	speedoPos = new D3DXVECTOR3();
	speedoPos->x = width - 150.0f;
	speedoPos->y = height - 150.0f;
	speedoPos->z = 0.0f;

	D3DXMatrixIdentity(&needleTrans);

	currentSpeed = 0;
	currentHealth = 100;
	currentCheckpointTime = 0;
	currentLap = 0;
}

void HUD::initialize(IDirect3DDevice9* device)
{
	D3DXCreateTextureFromFile(device, "radialMenu.dds", &radialMenuTexture);
	D3DXCreateTextureFromFile(device, "reticule.dds", &reticuleTexture);
	D3DXCreateTextureFromFile(device, "speedometer.dds", &speedoTexture);
	D3DXCreateTextureFromFile(device, "needle.dds", &needleTexture);
	D3DXCreateTextureFromFile(device, "LCD.dds", &numbersTexture);
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


	// Draw checkpoint timer
	drawCheckpointTime();

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
	drawPos.x = 96.0f;
	drawPos.y = screenHeight - 64.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 currCenter;
	currCenter.x = 8.0f;
	currCenter.y = 14.0f;
	currCenter.z = 0.0f;

	RECT current;

	current.top = 0;
	current.bottom = 28;
	current.left = 0;
	current.right = 16;

	for (int i = 0; i < 3; i++)
	{
		if (healthString[i] == '0')
		{
			current.left = 16 * 9;
			current.right = 16 * 10;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos, D3DCOLOR_XRGB(255, 50, 50));

			drawPos.x += 16.0f;
		}
		else if (healthString[i] == ':')
		{
			current.left = 16 * 10;
			current.right = 16 * 11;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos, D3DCOLOR_XRGB(255, 50, 50));

			drawPos.x += 16.0f;
		}
		else if ((healthString[i] > '0') && (healthString[i] <= '9'))
		{
			int num = (int) healthString[i] - '0';

			current.left = 16 * (num - 1);
			current.right = 16 * num;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(255, 50, 50));

			drawPos.x += 16.0f;
		}
	}
	
}

void HUD::drawCheckpointTime()
{
	char timeString[6];
	_itoa_s(currentCheckpointTime, timeString, 6, 10);

	D3DXVECTOR3 drawPos;
	drawPos.x = screenWidth / 2.0f - 32.0f;
	drawPos.y = 64.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 currCenter;
	currCenter.x = 8.0f;
	currCenter.y = 14.0f;
	currCenter.z = 0.0f;

	RECT current;

	current.top = 0;
	current.bottom = 28;
	current.left = 0;
	current.right = 16;

	for (int i = 0; i < 5; i++)
	{
		if (timeString[i] == '0')
		{
			current.left = 16 * 9;
			current.right = 16 * 10;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(0, 255, 255));

			drawPos.x += 16.0f;
		}
		else if (timeString[i] == ':')
		{
			current.left = 16 * 10;
			current.right = 16 * 11;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(0, 255, 255));

			drawPos.x += 16.0f;
		}
		else if ((timeString[i] > '0') && (timeString[i] <= '9'))
		{
			int num = (int) timeString[i] - '0';

			current.left = 16 * (num - 1);
			current.right = 16 * num;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(0, 255, 255));

			drawPos.x += 16.0f;
		}
		else
		{
			break;
		}
	}
	
}


void HUD::setCheckpointTime(int time)
{
	currentCheckpointTime = time;
}

void HUD::setLap(int lap)
{
	currentLap = lap;
}


void HUD::drawLap()
{
	char lapString[6];
	_itoa_s(currentLap, lapString, 6, 10);

	D3DXVECTOR3 drawPos;
	drawPos.x = screenWidth - 64.0f;
	drawPos.y = 64.0f;
	drawPos.z = 0.0f;

	D3DXVECTOR3 currCenter;
	currCenter.x = 8.0f;
	currCenter.y = 14.0f;
	currCenter.z = 0.0f;

	RECT current;

	current.top = 0;
	current.bottom = 28;
	current.left = 0;
	current.right = 16;

	for (int i = 0; i < 5; i++)
	{
		if (lapString[i] == '0')
		{
			current.left = 16 * 9;
			current.right = 16 * 10;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos, D3DCOLOR_XRGB(0, 0, 255));

			drawPos.x += 16.0f;
		}
		else if (lapString[i] == ':')
		{
			current.left = 16 * 10;
			current.right = 16 * 11;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(0, 0, 255));

			drawPos.x += 16.0f;
		}
		else if ((lapString[i] > '0') && (lapString[i] <= '9'))
		{
			int num = (int) lapString[i] - '0';

			current.left = 16 * (num - 1);
			current.right = 16 * num;

			sprite->Draw(numbersTexture, &current, &currCenter, &drawPos,  D3DCOLOR_XRGB(0, 0, 255));

			drawPos.x += 16.0f;
		}
		else
		{
			break;
		}
	}
	
}