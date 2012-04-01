#include "Rocket.h"


Rocket::Rocket(IDirect3DDevice9* device)
{
	drawable = new Drawable(ROCKETMESH, "rocket.dds", device);

	hkVector4 startAxis = hkVector4(0, 0, -0.5f);
	hkVector4 endAxis = hkVector4(0, 0, 0.5f);
	hkReal radius = 0.1f;

	hkpRigidBodyCinfo info;
	info.m_gravityFactor = 0.0f;
	info.m_friction = 0.0f;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	info.m_restitution = 0.0f;
	info.m_angularDamping = 1.0f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeCylinderVolumeMassProperties(startAxis, endAxis, radius, 1.0f, massProperties);
	info.setMassProperties(massProperties);
	
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	body->setAngularVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();
	
	
	listener = new RocketListener(this);
	body->addContactListener(listener);

	Physics::physics->addRigidBody(body);

	destroyed = false;

	emitter = Sound::sound->getEmitter();
	rocketVoice = Sound::sound->reserveSFXVoice();
}

Rocket::Rocket()
{

}

Rocket::~Rocket(void)
{
	if (listener)
	{
		body->removeContactListener(listener);
		delete listener;
	}
	
	if (body)
	{
		body->removeReference();
		
		Physics::world->removeEntity(body);
	}

	if (drawable)
	{
		delete drawable;
	}

	Sound::sound->returnEmitter();
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

		
		// Update 3D sound position
		hkVector4 vec = hkVector4(body->getPosition());

		emitter->Position.x = vec(0);
		emitter->Position.y = vec(1);
		emitter->Position.z = vec(2);

		vec = hkVector4(body->getLinearVelocity());

		emitter->Velocity.x = vec(0);
		emitter->Velocity.y = vec(1);
		emitter->Velocity.z = vec(2);

		Sound::sound->playEngine(emitter, 1.0f, rocketVoice);
	}
}

void Rocket::explode()
{
	destroyed = true;
	Sound::sound->playBoost(emitter);
}


RocketListener::RocketListener(Rocket* r)
{
	rocket = r;
}

void RocketListener::collisionAddedCallback(const hkpCollisionEvent& ev)
{
	rocket->explode();
}
