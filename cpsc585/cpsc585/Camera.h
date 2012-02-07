#pragma once
#pragma comment(lib, "d3d9.lib")

#include <d3dx9math.h>

#include "Drawable.h"


class Camera
{
public:
	Camera(void);
	~Camera(void);
	void setFocus(Drawable* focus);
	void update();

	void render();
	void getViewMatrix(D3DXMATRIX& matrix);

private:
	D3DXMATRIX viewMatrix;
	Drawable* focusObject;
	D3DXVECTOR3 up, lookAt, position;
	float kValue;
};
