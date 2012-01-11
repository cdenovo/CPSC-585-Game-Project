#include <Windows.h>

class Input
{
public:
	Input(void);
	~Input(void);
	void processWindowsMsg(UINT umessage, WPARAM wparam);
};

