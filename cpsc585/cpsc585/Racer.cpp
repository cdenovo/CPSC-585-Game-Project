#include "Racer.h"


Racer::Racer(IDirect3DDevice9* device)
{
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
}


Racer::~Racer(void)
{
	if(body)
	{
		body->removeReference();
	}
}

void Racer::setPosAndRot(float posX, float posY, float posZ,
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


void Racer::update()
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);
	}
}