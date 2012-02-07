#include "Input.h"


Input::Input(void)
{
	quit = false;
	debug = false;

	XINPUT_CAPABILITIES cap;
	
	if (XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &cap) == ERROR_SUCCESS)
	{
		controllerAvailable = true;
	}
	else
	{
		XINPUT_STATE state;
		HRESULT res = XInputGetState(0, &state);
		intention.reset();

		controllerAvailable = false;
	}
}


Input::~Input(void)
{
}


void Input::processWindowsMsg(UINT umessage, WPARAM wparam)
{
	if ((umessage == WM_KEYDOWN) && (wparam == 'Q'))
		quit = true;
	else if ((umessage == WM_KEYDOWN) && (wparam == 'D'))
		debug = !debug;
	else
	{
		// Process input

	}
	return;
}

// Returns TRUE if quitting, FALSE otherwise
bool Input::update()
{
	if (!controllerAvailable)
		return quit;

	XINPUT_STATE state;
	
	HRESULT res = XInputGetState(0, &state);

	intention.reset();

	if (SUCCEEDED(res))// && (state.Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		XINPUT_GAMEPAD gamepad = state.Gamepad;
		
		if(gamepad.wButtons & XINPUT_GAMEPAD_A){
			intention.aPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_B){
			intention.bPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_X){
			intention.xPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_Y){
			intention.yPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
			intention.rbumpPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER){
			intention.lbumpPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_START){
			intention.startPressed = true;
		}
		if(gamepad.wButtons & XINPUT_GAMEPAD_BACK){
			intention.selectPressed = true;
		}

		if (gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			intention.rightTrig = gamepad.bRightTrigger;
		}
		else
		{
			intention.rightTrig = 0;
		}

		if (gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			intention.leftTrig = gamepad.bLeftTrigger;
		}
		else
		{
			intention.leftTrig = 0;
		}
		
		if ((gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			|| (gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			intention.rightStickX = gamepad.sThumbRX;
		}
		else
		{
			intention.rightStickX = 0;
		}

		if ((gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			|| (gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			intention.rightStickY = gamepad.sThumbRY;
		}
		else
		{
			intention.rightStickY = 0;
		}
		
		
		if ((gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			|| (gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			intention.leftStick = gamepad.sThumbLX;
		}
		else
		{
			intention.leftStick = 0;
		}

		

		 /*Vibrating the controller:
			XINPUT_VIBRATION Vibration;
			Vibration.wLeftMotorSpeed = 50000;
			Vibration.wRightMotorSpeed = 50000;
			XInputSetState(0, &Vibration);
		*/
		// Process input based off controller's current state
	}
	
	return quit;
}

Intention Input::getIntention()
{
	return intention;
}

bool Input::debugging()
{
	return debug;
}