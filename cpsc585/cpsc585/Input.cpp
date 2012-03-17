#include "Input.h"


Input::Input(void)
{
	quit = false;
	debug = false;
	placeWaypoint = false;
	network = false;
	client = false;
	server = false;
	menu = false;

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
	else if ((umessage == WM_KEYDOWN) && (wparam == 'M'))
	{
		menu = !menu;
		network = false;
		debug = false;
	}
	else if ((umessage == WM_KEYDOWN) && (wparam == 'B'))
	{
		debug = !debug;
		network = false;
		menu = false;
	}
	else if ((umessage == WM_KEYDOWN) && (wparam == 'N'))
	{
		debug = false;
		menu = false;
		network = !network;
	}
	else if ((umessage == WM_KEYDOWN) && (wparam == 'C') && network && !server)
		client = true;
	else if ((umessage == WM_KEYDOWN) && (wparam == 'V') && network && !client)
		server = true;
	else if ((umessage == WM_KEYUP) && (wparam == 'X'))
		placeWaypoint = true;
	else
	{
		// Process input
		if (umessage == WM_KEYDOWN)
		{
			switch (wparam)
			{
			case VK_UP:
				{
					intention.rightStickY = 25000;
					break;
				}
			case VK_DOWN:
				{
					intention.rightStickY = -25000;
					break;
				}
			case VK_LEFT:
				{
					intention.rightStickX = -25000;
					break;
				}
			case VK_RIGHT:
				{
					intention.rightStickX = 25000;
					break;
				}
			case VK_SPACE:
					{
						intention.rightTrig = 255;
						break;
					}
				case VK_SHIFT:
					{
						intention.leftTrig = 255;
						break;
					}
				case 'Z':
					{
						intention.yPressed = true;
						break;
					}
				case 'R':
					{
						intention.rbumpPressed = true;
						break;
					}
				case 'E':
					{
						intention.lbumpPressed = true;
						break;
					}
				case 'A':
					{
						intention.leftStick = -25000;
						break;
					}
				case 'D':
					{
						intention.leftStick = 25000;
						break;
					}
			}
		}
		else if (umessage == WM_KEYUP)
		{
			switch (wparam)
			{
			case VK_UP:
				{
					intention.rightStickY = 0;
					break;
				}
			case VK_DOWN:
				{
					intention.rightStickY = 0;
					break;
				}
			case VK_LEFT:
				{
					intention.rightStickX = 0;
					break;
				}
			case VK_RIGHT:
				{
					intention.rightStickX = 0;
					break;
				}
			case VK_SPACE:
				{
					intention.rightTrig = 0;
					break;
				}
			case VK_SHIFT:
				{
					intention.leftTrig = 0;
					break;
				}
			case 'Z':
				{
					intention.yPressed = false;
					break;
				}
			case 'R':
				{
					intention.rbumpPressed = false;
					break;
				}
			case 'E':
				{
					intention.lbumpPressed = false;
					break;
				}
			case 'A':
				{
					intention.leftStick = 0;
					break;
				}
			case 'D':
				{
					intention.leftStick = 0;
					break;
				}
			}
		}
		// Compute acceleration and steering (between -1.0 and 1.0)
		intention.acceleration = (intention.rightTrig - intention.leftTrig) / (float) (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		intention.steering = intention.leftStick / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	return;
}

// Returns TRUE if quitting, FALSE otherwise
bool Input::update()
{
	XINPUT_CAPABILITIES cap;

	if (XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &cap) != ERROR_SUCCESS)
	{
		controllerAvailable = false;

		return quit;
	}
	else
	{
		controllerAvailable = true;
	}

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

		if (gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			intention.rightTrig = gamepad.bRightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
		else
		{
			intention.rightTrig = 0;
		}

		if (gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			intention.leftTrig = gamepad.bLeftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
		else
		{
			intention.leftTrig = 0;
		}
		
		if (gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			intention.rightStickX = gamepad.sThumbRX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		else if (gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			intention.rightStickX = gamepad.sThumbRX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		else
		{
			intention.rightStickX = 0;
		}

		if (gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			intention.rightStickY = gamepad.sThumbRY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		else if (gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			intention.rightStickY = gamepad.sThumbRY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		else
		{
			intention.rightStickY = 0;
		}
		

		if (gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			intention.leftStick = gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else if (gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			intention.leftStick = gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else
		{
			intention.leftStick = 0;
		}

		
		// Compute acceleration and steering (between -1.0 and 1.0)
		intention.acceleration = (intention.rightTrig - intention.leftTrig) / (float) (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		intention.steering = intention.leftStick / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		



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

bool Input::placingWaypoint()
{
	return placeWaypoint;
}

void Input::setPlaceWaypointFalse()
{
	placeWaypoint = false;
}

bool Input::networking()
{
	return network;
}

bool Input::isClient()
{
	return client;
}

bool Input::isServer()
{
	return server;
}

bool Input::menuOn()
{
	return menu;
}

bool Input::quitOn()
{
	return quit;
}