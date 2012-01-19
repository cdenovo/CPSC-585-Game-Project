#include "Input.h"


Input::Input(void)
{
	quit = false;
}


Input::~Input(void)
{
}


void Input::processWindowsMsg(UINT umessage, WPARAM wparam)
{
	if ((umessage == WM_KEYDOWN) && (wparam == 'Q'))
		quit = true;
	else
	{
		// Process input

	}
	return;
}

// Returns TRUE if quitting, FALSE otherwise
bool Input::update()
{
	XINPUT_STATE state;
	
	HRESULT res = XInputGetState(0, &state);

	if (SUCCEEDED(res))// && (state.Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		// Process input based off controller's current state
	}
	
	return quit;
}
