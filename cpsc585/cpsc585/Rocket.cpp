#include "Rocket.h"


Rocket::Rocket(IDirect3DDevice9* device)
{
	drawable = new Drawable(ROCKETMESH, "rocket.dds", device);

	hkVector4 startAxis = hkVector4(0, 0, -0.5f);
	hkVector4 endAxis = hkVector4(0, 0, 0.5f);
	hkReal radius = 0.1f;

	hkpRigidBodyCinfo info;
	info.m_gravityFactor = 0.0f;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	info.m_restitution = 0.0f;
	info.m_angularDamping = 1.0f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeCylinderVolumeMassProperties(startAxis, endAxis, radius, 50.0f, massProperties);
	info.setMassProperties(massProperties);

	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	body->setAngularVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();
	
	Physics::addRigidBody(body);

	destroyed = false;
}

Rocket::Rocket()
{

}

Rocket::~Rocket(void)
{
	if (body)
	{
		body->removeReference();
	}
}

void Rocket::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 0.0f, 1.0f), rotZ));

	hkVector4 pos = hkVector4(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}

void Rocket::update()
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);

		/*
		// Update 3D sound position
		hkVector4 vec = hkVector4(lookDir);
		vec.normalize3();
		vec(0) = vec(0);
		emitter->OrientFront.x = vec(0);
		emitter->OrientFront.y = vec(1);
		emitter->OrientFront.z = vec(2);

		vec = hkVector4(body->getPosition());

		emitter->Position.x = vec(0);
		emitter->Position.y = vec(1);
		emitter->Position.z = vec(2);

		vec = hkVector4(body->getLinearVelocity());

		emitter->Velocity.x = vec(0);
		emitter->Velocity.y = vec(1);
		emitter->Velocity.z = vec(2);*/
	}
}

void Rocket::explode()
{

}
