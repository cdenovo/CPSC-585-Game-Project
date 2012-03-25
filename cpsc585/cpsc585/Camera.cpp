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
	D3DXVECTOR3 zVec, objPos, desiredPos;

	objPos = focusObject->getPosition();
	zVec = focusObject->getZVector();

	position = objPos + (lookDir * -9.0f);
	position.y += 2.0f;

	lookAt = D3DXVECTOR3(position + lookDir);

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);


	/*
	D3DXVECTOR3 zVec, objPos, desiredPos;

	objPos = focusObject->getPosition();
	zVec = focusObject->getZVector();

	lookAt += (((zVec * (20.0f)) + objPos) - lookAt) / kValue;

	desiredPos = (zVec * (-9.0f)) + objPos;
	desiredPos.y += 2.0f;

	position += (desiredPos - position) / kValue;

	lookAt = D3DXVECTOR3(position + lookDir);

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	*/
}


void Camera::setLookDir(float x, float y, float z)
{
	lookDir.x = x;
	lookDir.y = y;
	lookDir.z = z;
}
