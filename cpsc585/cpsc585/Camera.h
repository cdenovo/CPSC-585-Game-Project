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
	void updateForSkybox();

	void render();
	void getViewMatrix(D3DXMATRIX& matrix);
	void setLookDir(float x, float y, float z);

private:
	D3DXMATRIX viewMatrix;
	Drawable* focusObject;
	D3DXVECTOR3 up, lookAt, position, lookDir;
	float kValue;
};
