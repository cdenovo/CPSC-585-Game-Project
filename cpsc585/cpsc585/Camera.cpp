#include "Camera.h"


Camera::Camera(void)
{
	kValue = 20.0f;

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

	lookAt = (zVec * (30.0f)) + objPos;

	position = (zVec * (-12.0f)) + objPos;
	position.y += 3.0f;

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void Camera::update()
{
	D3DXVECTOR3 zVec, objPos, desiredPos;

	objPos = focusObject->getPosition();
	zVec = focusObject->getZVector();

	lookAt += (((zVec * (30.0f)) + objPos) - lookAt) / kValue;

	desiredPos = (zVec * (-12.0f)) + objPos;
	desiredPos.y += 3.0f;

	position += (desiredPos - position) / kValue;

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}