#include "Waypoint.h"



Waypoint::Waypoint(IDirect3DDevice9* device)
{
	radius = 20;

	drawable = new Drawable(RACER, "someTextureName", device);

	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(1.0f, 1.0f, 2.5f);		//Half extent for racer rigid body box
	info.m_shape = new hkpBoxShape(halfExtent);
	info.m_restitution = 0.2f;
	const hkReal boxMass = 1000.0f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(info.m_shape, boxMass, massProperties);
	info.setMassProperties(massProperties);
	body = new hkpRigidBody(info);		//Create rigid body

	//body = NULL;
}


Waypoint::~Waypoint(void)
{
	if(body)
	{
		body->removeReference();
	}
}

void Waypoint::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotZ));

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}


void Waypoint::update()
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);
	}
}

float Waypoint::getRadius()
{
	return radius;
}

void Waypoint::setRadius(float newRadius)
{
	radius = newRadius;
}

bool Waypoint::withinWaypoint(const hkVector4* position)
{
	hkVector4 wpPosition = body->getPosition();
	hkSimdReal distance = wpPosition.distanceTo(*position);
	hkBool32 distanceLessThanRadius = distance.isLess(radius);
	if(distanceLessThanRadius){
		return true;
	}
	else{
		return false;
	}
}