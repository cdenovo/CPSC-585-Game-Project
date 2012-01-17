#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

#include "Renderer.h"
#include "AI.h"
#include "Input.h"
#include "Sound.h"

Renderer* renderer;
AI* ai;
Input* input;
Sound* sound;

LPCSTR appName;
HINSTANCE hInstance;
HWND hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
bool initialize();
void run();
void shutdown();
bool mainLoop();

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
