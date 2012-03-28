#include "Camera.h"


Camera::Camera(void)
{
	kValue = 15.0f;

	focusObject = NULL;
}


Camera::~Camera(void)
{
}

void Camera::getViewMatrix(D3DXMATRIX& matrix)
{
	matrix = viewMatrix;
	return;
}

void Camera::setFocus(Drawable* focus)
{
	D3DXVECTOR3 zVec, objPos;

	focusObject = focus;

	objPos = focusObject->getPosition();
	zVec = focusObject->getZVector();

	lookAt = (zVec * (20.0f)) + objPos;

	position = (zVec * (-9.0f)) + objPos;
	position.y += 2.0f;

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void Camera::update()
{
	position = focusObject->getPosition();

	position = position + (lookDir * -7.0f);
	position.y += 2.0f;

	lookAt = D3DXVECTOR3(position + lookDir);

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}


void Camera::setLookDir(float x, float y, float z)
{
	lookDir.x = x;
	lookDir.y = y;
	lookDir.z = z;
}

void Camera::updateForSkybox()
{
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	lookAt = D3DXVECTOR3(lookDir);

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}
