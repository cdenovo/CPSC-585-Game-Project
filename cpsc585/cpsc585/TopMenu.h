#pragma once

#include "Input.h"
#include "Intention.h"

const int COUNTERTIME = 10;

enum StateSpace {STARTSEL, QUITSEL, STARTGAME, QUIT};

class TopMenu
{
public:
	TopMenu(Input *in);
	~TopMenu();

	void update();
	StateSpace getState();

	std::string str();

private:
	bool upPressed;
	bool downPressed;

	int counter;

	StateSpace state;

	Input *input;
};

