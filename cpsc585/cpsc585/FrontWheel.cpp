#include "FrontWheel.h"


FrontWheel::FrontWheel(IDirect3DDevice9* device, int filter)
{
	drawable = new Drawable(FRONTWHEEL, "tire.dds", device);

	hkVector4 startAxis = hkVector4(-0.3f, 0, 0);
	hkVector4 endAxis = hkVector4(0.3f, 0, 0);
	hkReal radius = 0.5f;


	hkpRigidBodyCinfo info;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_restitution = 1.0f;
	info.m_friction = 1.0f;
	info.m_angularDamping = 1.0f;
	hkReal wheelMass = 75.0f;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL; 
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeCylinderVolumeMassProperties(startAxis, endAxis, radius, wheelMass, massProperties);
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_DYNAMIC, filter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();


	
}


FrontWheel::~FrontWheel(void)
{
	if (body)
	{
		body->removeReference();
	}
}
