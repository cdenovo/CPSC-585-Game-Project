#pragma comment(lib, "XInput.lib")

#include <Windows.h>
#include <XInput.h>

#ifndef _INPUT_H
#define _INPUT_H
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
#endif