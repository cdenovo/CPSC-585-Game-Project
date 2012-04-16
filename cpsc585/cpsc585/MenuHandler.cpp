#include "MenuHandler.h"


MenuHandler::MenuHandler(int width, int height)
{
	screenWidth = width;
	screenHeight = height;

	sprite = NULL;
	uiTexture = NULL;
	mainMenuTexture = NULL;
	loadingbgTexture = NULL;
	loadingTextTexture = NULL;

	selectedOption = START_GAME;
	selectedMode = MAIN_MENU;

	arrowRect = NULL;
	startGameRect = NULL;
	continueRect = NULL;
	quitRect = NULL;

	enabled = false;
	loadingComplete = false;
}


MenuHandler::~MenuHandler()
{
}

void MenuHandler::initialize(IDirect3DDevice9* device)
{
	arrowRect = new RECT();
	startGameRect = new RECT();
	continueRect = new RECT();
	quitRect = new RECT();

	arrowRect->top = 0;
	arrowRect->bottom = 128;
	arrowRect->left = 0;
	arrowRect->right = 128;

	startGameRect->top = 128;
	startGameRect->bottom = 256;
	startGameRect->left = 0;
	startGameRect->right = 512;

	continueRect->top = 256;
	continueRect->bottom = 384;
	continueRect->left = 0;
	continueRect->right = 512;

	quitRect->top = 384;
	quitRect->bottom = 512;
	quitRect->left = 0;
	quitRect->right = 512;

	D3DXCreateTextureFromFile(device, "textures/mainmenu.dds", &mainMenuTexture);
	D3DXCreateTextureFromFile(device, "textures/ui.dds", &uiTexture);
	D3DXCreateTextureFromFile(device, "textures/loadingbg.dds", &loadingbgTexture);
	D3DXCreateTextureFromFile(device, "textures/loadingText.dds", &loadingTextTexture);

	D3DXCreateSprite(device, &sprite);

	std::srand((unsigned int) time(NULL));
}

void MenuHandler::shutdown()
{
	if (arrowRect)
	{
		delete arrowRect;
		arrowRect = NULL;
	}

	if (startGameRect)
	{
		delete startGameRect;
		startGameRect = NULL;
	}

	if (continueRect)
	{
		delete continueRect;
		continueRect = NULL;
	}

	if (quitRect)
	{
		delete quitRect;
		quitRect = NULL;
	}

	if (mainMenuTexture)
	{
		mainMenuTexture->Release();
		mainMenuTexture = NULL;
	}

	if (uiTexture)
	{
		uiTexture->Release();
		uiTexture = NULL;
	}
}

void MenuHandler::render()
{
	if (!enabled)
		return;
	
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	IDirect3DDevice9* device;
	sprite->GetDevice(&device);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);

	if (selectedMode == MAIN_MENU)
	{
		int jitter = std::rand() % 100;
		
		if (jitter < 95)
			jitter = 0;
		else
			jitter = std::rand() % 100 - 50;

		// Draw main menu
		D3DXVECTOR3 currentPos;
		currentPos.x = (float) screenWidth / 2.0f - 192 + jitter;
		currentPos.y = (float) screenHeight / 2.0f + jitter;
		currentPos.z = 0;

		float scaleFactorX = screenWidth / 1024.0f;
		float scaleFactorY = screenHeight / 819.0f;

		D3DXMATRIX origTrans, newTrans;
		sprite->GetTransform(&origTrans);
		D3DXMatrixScaling(&newTrans, scaleFactorX, scaleFactorY, 1.0f);
		sprite->SetTransform(&newTrans);

		RECT area;
		area.top = 0;
		area.bottom = 819;
		area.left = 0;
		area.right = 1024;

		sprite->Draw(mainMenuTexture, &area, &D3DXVECTOR3(512, 409.5f, 0),
			&D3DXVECTOR3(512, 409.5f, 0), 0xFFFFFFFF);

		sprite->SetTransform(&origTrans);


		// Draw menu options
		sprite->Draw(uiTexture, startGameRect, NULL, &currentPos, 0xFFFFFFFF);
		currentPos.y += 128;
		sprite->Draw(uiTexture, quitRect, NULL, &currentPos, 0xFFFFFFFF);
		
		// Draw arrow next to appropriate selection
		currentPos.x -= 128.0f;
		currentPos.y = (float) screenHeight / 2.0f;

		if (selectedOption == QUIT)
		{
			currentPos.y += 128;
		}

		sprite->Draw(uiTexture, arrowRect, NULL, &currentPos, 0xFFFF0000);
	}
	else if (selectedMode == PAUSE_MENU)
	{
		int jitter = std::rand() % 100;
		
		if (jitter < 95)
			jitter = 0;
		else
			jitter = std::rand() % 100 - 50;

		// Draw pause menu
		D3DXVECTOR3 currentPos;
		currentPos.x = (float) screenWidth / 2 - 128 + jitter;
		currentPos.y = (float) screenHeight / 2 - 128 + jitter;
		currentPos.z = 0;

		// Draw menu options
		sprite->Draw(uiTexture, continueRect, NULL, &currentPos, 0xFFFFFFFF);
		currentPos.y += 128;
		sprite->Draw(uiTexture, quitRect, NULL, &currentPos, 0xFFFFFFFF);
		
		// Draw arrow next to appropriate selection
		currentPos.x -= 128.0f;
		currentPos.y = (float) screenHeight / 2 - 128;

		if (selectedOption == CONTINUE)
		{
			// Do nothing
		}
		else if (selectedOption == QUIT)
		{
			currentPos.y += 128;
		}

		sprite->Draw(uiTexture, arrowRect, NULL, &currentPos, 0xFFFF0000);
	}
	else if (selectedMode == LOADING)
	{
		// Show loading screen, then wait for player to hit A or Start
		sprite->Draw(loadingbgTexture, NULL, &D3DXVECTOR3(512, 0, 0),
			&D3DXVECTOR3((float) screenWidth / 2.0f, 0, 0), 0xFFFFFFFF);
		
		
		if (loadingComplete)
		{
			RECT doneLoading;
			doneLoading.top = 128;
			doneLoading.bottom = 256;
			doneLoading.left = 0;
			doneLoading.right = 512;

			sprite->Draw(loadingTextTexture, &doneLoading, &D3DXVECTOR3(256, 0, 0),
				&D3DXVECTOR3((float) screenWidth / 2.0f, (float) screenHeight - 128.0f, 0), 0xFFFFFFFF);
		}
		else
		{
			RECT loading;
			loading.top = 0;
			loading.bottom = 128;
			loading.left = 0;
			loading.right = 512;

			sprite->Draw(loadingTextTexture, &loading, &D3DXVECTOR3(256, 0, 0),
				&D3DXVECTOR3((float) screenWidth / 2.0f, (float) screenHeight - 128.0f, 0), 0xFFFFFFFF);
		}
	}


	sprite->End();
}

void MenuHandler::setSelectedOption(SelectedOption option)
{
	if (selectedOption != option)
		Sound::sound->playSoundEffect(SFX_SELECT, Sound::sound->playerEmitter);

	selectedOption = option;
}

SelectedOption MenuHandler::getSelectedOption()
{
	return selectedOption;
}

void MenuHandler::setMode(SelectedMode mode)
{
	selectedMode = mode;

	if (selectedMode == MAIN_MENU)
		selectedOption = START_GAME;
	else if (selectedMode == PAUSE_MENU)
		selectedOption = CONTINUE;
}

void MenuHandler::moveUp()
{
	if (selectedMode == MAIN_MENU)
	{
		if (selectedOption == QUIT)
			setSelectedOption(START_GAME);

	}
	else if (selectedMode == PAUSE_MENU)
	{
		if (selectedOption == QUIT)
			setSelectedOption(CONTINUE);
	}
}

void MenuHandler::moveDown()
{
	if (selectedMode == MAIN_MENU)
	{
		if (selectedOption == START_GAME)
			setSelectedOption(QUIT);

	}
	else if (selectedMode == PAUSE_MENU)
	{
		if (selectedOption == CONTINUE)
			setSelectedOption(QUIT);
	}
}

SelectedMode MenuHandler::getMode()
{
	return selectedMode;
}
