#pragma once
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

// AI.h includes everything Main needs to know about
#include "AI.h"

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

bool quit;

ULONGLONG prevTime;