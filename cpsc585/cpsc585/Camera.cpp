#include "Camera.h"


Camera::Camera(void)
{
	posX = 0.0f;
	posY = 0.0f;
	posZ = 0.0f;

	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
}


Camera::~Camera(void)
{
}

void Camera::setPos(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	return;
}

void Camera::setRot(float x, float y, float z)
{
	rotX = x;
	rotY = y;
	rotZ = z;
	return;
}

D3DXVECTOR3 Camera::getPos()
{
	return D3DXVECTOR3(posX, posY, posZ);
}

D3DXVECTOR3 Camera::GetRot()
{
	return D3DXVECTOR3(rotX, rotY, rotZ);
}

void Camera::render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = posX;
	position.y = posY;
	position.z = posZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotX * 0.0174532925f;
	yaw   = rotY * 0.0174532925f;
	roll  = rotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	return;
}

void Camera::getViewMatrix(D3DXMATRIX& matrix)
{
	matrix = viewMatrix;
	return;
}