#pragma once
#pragma comment(lib, "XInput.lib")

#include <Windows.h>
#include <XInput.h>

class Input
{
public:
	Input(void);
	~Input(void);
	void processWindowsMsg(UINT umessage, WPARAM wparam);
	bool update();

private:
	bool quit;
};
