#pragma comment(lib, "d3d9.lib")

#include <d3dx9math.h>

#ifndef _CAMERA_H
#define _CAMERA_H
class Camera
{
public:
	Camera(void);
	~Camera(void);
	void setPos(float x, float y, float z);
	void setRot(float x, float y, float z);

	D3DXVECTOR3 getPos();
	D3DXVECTOR3 GetRot();

	void render();
	void getViewMatrix(D3DXMATRIX& matrix);

private:
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	D3DXMATRIX viewMatrix;
};
#endif