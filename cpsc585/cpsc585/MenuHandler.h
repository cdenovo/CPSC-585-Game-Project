#pragma once
#include "Intention.h"
#include "Drawable.h"
#include "Sound.h"

enum SelectedOption { START_GAME, CONTINUE, QUIT };
enum SelectedMode { MAIN_MENU, PAUSE_MENU, LOADING };

class MenuHandler
{
public:
	MenuHandler(int width, int height);
	~MenuHandler();
	void initialize(IDirect3DDevice9* device);
	void update(Intention intention);
	void render();
	void shutdown();
	void setSelectedOption(SelectedOption option);
	SelectedOption getSelectedOption();
	void setMode(SelectedMode mode);
	SelectedMode getMode();
	void moveUp();
	void moveDown();

	bool enabled;
	bool loadingComplete;

private:
	IDirect3DTexture9* uiTexture;
	IDirect3DTexture9* mainMenuTexture;
	IDirect3DTexture9* loadingbgTexture;
	IDirect3DTexture9* loadingTextTexture;

	ID3DXSprite* sprite;

	int screenWidth, screenHeight;

	SelectedOption selectedOption;
	SelectedMode selectedMode;

	RECT* arrowRect;
	RECT* startGameRect;
	RECT* continueRect;
	RECT* quitRect;
};
