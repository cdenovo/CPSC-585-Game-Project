#include "RearWheel.h"


RearWheel::RearWheel(IDirect3DDevice9* device, int filter)
{
	drawable = new Drawable(REARWHEEL, "tire.dds", device);

	hkVector4 startAxis = hkVector4(-0.4f, 0, 0);
	hkVector4 endAxis = hkVector4(0.4f, 0, 0);
	hkReal radius = 0.7f;


	hkpRigidBodyCinfo info;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_restitution = 1.0f;
	info.m_friction = 4.0f;
	info.m_angularDamping = 4.0f;
	hkReal wheelMass = 150.0f;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL; 
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeCylinderVolumeMassProperties(startAxis, endAxis, radius, wheelMass, massProperties);
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_DYNAMIC, filter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();


	
}


RearWheel::~RearWheel(void)
{
	if (body)
	{
		body->removeReference();
	}
}

void RearWheel::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotZ));

	hkVector4 pos = hkVector4(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}
