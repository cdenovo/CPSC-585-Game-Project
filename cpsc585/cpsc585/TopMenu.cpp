#include "TopMenu.h"


TopMenu::TopMenu(Input *in)
{
	input = in;
	upPressed = false;
	downPressed = false;
	counter = 0;
	state = STARTSEL;
}


TopMenu::~TopMenu()
{
}

void TopMenu::update()
{
	Intention intent = input->getIntention();

	if(intent.aPressed)
	{
		switch (state)
		{
		case STARTSEL:
			state = STARTGAME;
			break;
		case QUITSEL:
			state = QUIT;
			break;
		}

		upPressed = false;
		downPressed = false;
	}
	else if(!upPressed && intent.rightStickY > 0) //If the player presses the right stick up
	{
		switch (state)
		{
		case QUITSEL:
			state = STARTSEL;
			break;
		}

		upPressed = true;
		downPressed = false;
		counter = COUNTERTIME;
	}
	else if(!downPressed && intent.rightStickY < 0)
	{
		switch (state)
		{
		case STARTSEL:
			state = QUITSEL;
			break;
		}

		upPressed = false;
		downPressed = true;
		counter = COUNTERTIME;
	}
	else if(counter > 0)
	{
		upPressed = false;
		downPressed = false;
		counter--;
	}
}

std::string TopMenu::str()
{
	std::string s = "";
	switch(state)
	{
	case STARTSEL:
		{
			s = "Start Game is selected.";
			break;
		}
	case QUITSEL:
		{
			s = "Quit Game is selected.";
			break;
		}
	case STARTGAME:
		{
			s = "The game is starting.";
			break;
		}
	case QUIT:
		{
			s = "Quitting game.";
			break;
		}
	}

	return s;
}

StateSpace TopMenu::getState()
{
	return state;
}