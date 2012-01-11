#include "Input.h"


Input::Input(void)
{
}


Input::~Input(void)
{
}


void Input::processWindowsMsg(UINT umessage, WPARAM wparam)
{
	if ((umessage == WM_KEYDOWN) && (wparam == 'Q'))
		PostQuitMessage(0);
	return;
}