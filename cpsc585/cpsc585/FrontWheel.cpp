#include "FrontWheel.h"


FrontWheel::FrontWheel(IDirect3DDevice9* device, int filter)
{
	touchingGround = false;

	drawable = new Drawable(FRONTWHEEL, "textures/tire.dds", device);

	hkVector4 startAxis;
	startAxis.set(-0.1f, 0, 0);

	hkVector4 endAxis;
	endAxis.set(0.1f, 0, 0);

	hkReal radius = 0.35f;

	hkpRigidBodyCinfo info;
	info.m_gravityFactor = 0.0f;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	info.m_restitution = 0.0f;
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_DYNAMIC, filter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();

	lastPos.set(0,0,0);
	rotation = 0.0;
}


FrontWheel::~FrontWheel(void)
{
	if (body)
	{
		body->removeReference();
	}
}


void FrontWheel::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 0.0f, 1.0f), rotZ));

	hkVector4 pos;
	pos.set(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}
