#include "Camera.h"


Camera::Camera(void)
{
	kValue = 7.0f;

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
	D3DXVECTOR3 zVec, xVec, objPos, forward;

	focusObject = focus;

	objPos = focusObject->getPosition();
	xVec = focusObject->getXVector();
	zVec = focusObject->getZVector();

	lookAt = (zVec * (30.0f)) + objPos;

	position = (zVec * (-10.0f)) + objPos;
	position.y += 3.0f;

	D3DXVec3Normalize(&forward, &(lookAt - position));

	forward = zVec;
	D3DXVec3Cross(&up, &forward, &xVec);
}

void Camera::update()
{
	D3DXVECTOR3 zVec, xVec, objPos, forward, desiredPos;

	objPos = focusObject->getPosition();
	xVec = focusObject->getXVector();
	zVec = focusObject->getZVector();

	lookAt += (((zVec * (30.0f)) + objPos) - lookAt) / kValue;

	desiredPos = (zVec * (-10.0f)) + objPos;
	desiredPos.y += 3.0f;

	position += (desiredPos - position) / kValue;

	D3DXVec3Normalize(&forward, &(lookAt - position));

	forward = zVec;
	D3DXVec3Cross(&up, &forward, &xVec);
	
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}