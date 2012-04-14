#include "Input.h"


Input::Input(void)
{
	quit = false;
	debug = false;
	placeWaypoint = false;

	XINPUT_CAPABILITIES cap;
	
	if (XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &cap) == ERROR_SUCCESS)
	{
		controllerAvailable = true;
	}
	else
	{
		controllerAvailable = false;
	}

	intention.reset();
}


Input::~Input(void)
{
}


void Input::processWindowsMsg(UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (umessage == WM_INPUT)
	{
		UINT size = 40;	// This is sizeof(RAWINPUT)
		static BYTE rawBytes[40];

		GetRawInputData((HRAWINPUT) lparam, RID_INPUT, rawBytes, &size, sizeof(RAWINPUTHEADER));
			
		RAWINPUT* raw = (RAWINPUT*) rawBytes;

		if (raw->header.dwType == RIM_TYPEKEYBOARD) 
		{
			// PROCESS KEYBOARD INPUT
			raw->data.keyboard.VKey = raw->data.keyboard.VKey;

			umessage = raw->data.keyboard.Message;
			wparam = raw->data.keyboard.VKey;

			if ((umessage == WM_KEYDOWN) && (wparam == 'Q'))
				quit = true;
			else if ((umessage == WM_KEYDOWN) && (wparam == 'B'))
				debug = !debug;
			else if ((umessage == WM_KEYUP) && (wparam == 'Z')){
				wpType = WAY_POINT;
				placeWaypoint = true;
			}
			else if ((umessage == WM_KEYUP) && (wparam == 'X')){
				wpType = TURN_POINT;
				placeWaypoint = true;
			}
			else if ((umessage == WM_KEYUP) && (wparam == 'C')){
				wpType = SHARP_POINT;
				placeWaypoint = true;
			}
			else if (umessage == WM_KEYDOWN)
			{
				switch (wparam)
				{
				case VK_UP:
					{
						intention.rightStickY = THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
						break;
					}
				case VK_DOWN:
					{
						intention.rightStickY = -THUMBSTICK_MAX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
						break;
					}
				case VK_LEFT:
					{
						intention.rightStickX = -THUMBSTICK_MAX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
						break;
					}
				case VK_RIGHT:
					{
						intention.rightStickX = THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
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
						intention.leftStickX = -THUMBSTICK_MAX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
						break;
					}
				case 'D':
					{
						intention.leftStickX = THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
						break;
					}
				case 'S':
					{
						intention.leftStickY = -THUMBSTICK_MAX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
						break;
					}
				case 'W':
					{
						intention.leftStickY = THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
						break;
					}
				case VK_RETURN:
					{
						intention.aPressed = true;
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
							intention.leftStickX = 0;
							break;
						}
					case 'D':
						{
							intention.leftStickX = 0;
							break;
						}
					case 'S':
						{
							intention.leftStickY = 0;
							break;
						}
					case 'W':
						{
							intention.leftStickY = 0;
							break;
						}
					case VK_RETURN:
						{
							intention.aPressed = false;
							break;
						}
				}
			}
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE) 
		{
			USHORT buttonInfo = raw->data.mouse.usButtonFlags;

			// Process mouse movement data
			if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				int xVal = raw->data.mouse.lLastX;
				int yVal = raw->data.mouse.lLastY * -1;

				if (xVal != 0)
				{
					intention.rightStickX = 3000 * xVal;
				}
				else
				{
					intention.rightStickX = 0;
				}

				if (yVal != 0)
				{
					intention.rightStickY = 3000 * yVal;
				}
				else
				{
					intention.rightStickY = 0;
				}
			}
			else
			{
				intention.rightStickX = 0;
				intention.rightStickY = 0;
			}
			

			// Process mouse clicks
			if (buttonInfo == RI_MOUSE_LEFT_BUTTON_DOWN)
				intention.rightTrig = 255;
			else if (buttonInfo == RI_MOUSE_LEFT_BUTTON_UP)
				intention.rightTrig = 0;
			else if (buttonInfo == RI_MOUSE_RIGHT_BUTTON_DOWN)
				intention.lbumpPressed = true;
			else if (buttonInfo == RI_MOUSE_RIGHT_BUTTON_UP)
				intention.lbumpPressed = false;

		}
		else
		{
			DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
		}
	}

	// Compute acceleration and steering (between -1.0 and 1.0)
	intention.acceleration = intention.leftStickY / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	intention.cameraX = intention.rightStickX / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	intention.cameraY = intention.rightStickY / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	

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
			intention.leftStickX = gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else if (gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			intention.leftStickX = gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else
		{
			intention.leftStickX = 0;
		}

		if (gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			intention.leftStickY = gamepad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else if (gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			intention.leftStickY = gamepad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		else
		{
			intention.leftStickY = 0;
		}

		
		// Compute acceleration and steering (between -1.0 and 1.0)
		intention.acceleration = intention.leftStickY / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		intention.cameraX = intention.rightStickX / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		intention.cameraY = intention.rightStickY / (float) (THUMBSTICK_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);


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
	if (!controllerAvailable)
	{
		Intention returnIntention = Intention(intention);

		intention.rightStickX = 0;
		intention.rightStickY = 0;
		intention.cameraX = 0;
		intention.cameraY = 0;

		return returnIntention;
	}

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
