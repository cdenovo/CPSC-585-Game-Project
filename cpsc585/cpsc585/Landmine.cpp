#include "Landmine.h"


Landmine::Landmine(IDirect3DDevice9* device)
{
	drawable = new Drawable(LANDMINEMESH, "textures/landmine.dds", device);

	hkVector4 startAxis;
	startAxis.set(0, -0.05f, 0);

	hkVector4 endAxis;
	endAxis.set(0, 0.05f, 0);

	hkReal radius = 0.6f;

	hkpRigidBodyCinfo info;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	info.m_angularDamping = 1.0f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeCylinderVolumeMassProperties(startAxis, endAxis, radius, 20.0, massProperties);
	info.setMassProperties(massProperties);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	body->setAngularVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();

	hkpPropertyValue val;
	val.setPtr(this);
	body->setProperty(1, val);
	
	
	listener = new LandmineListener(this);
	body->addContactListener(listener);

	Physics::physics->addRigidBody(body);

	destroyed = false;
	triggered = false;

	emitter = Sound::sound->getEmitter();
	
	owner = NULL;

	triggeredTime = 0.1f;
	activationTime = 1.0f;
	activated = false;
}



Landmine::~Landmine(void)
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

void Landmine::setPosAndRot(float posX, float posY, float posZ,
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

void Landmine::update(float seconds)
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);

		// Update 3D sound position
		hkVector4 vec;
		vec.setXYZ(body->getPosition());

		emitter->Position.x = vec(0);
		emitter->Position.y = vec(1);
		emitter->Position.z = vec(2);

		vec.setXYZ(body->getLinearVelocity());

		emitter->Velocity.x = vec(0);
		emitter->Velocity.y = vec(1);
		emitter->Velocity.z = vec(2);

		
		if (!activated)
		{
			activationTime -= seconds;
		
			if (activationTime <= 0.0f)
			{
				activated = true;
				Sound::sound->playSoundEffect(SFX_BEEP, emitter);
			}
		}
		else if (triggered)
		{
			triggeredTime -= seconds;

			if (triggeredTime <= 0.0f)
			{
				explode();
			}
		}
	}
}

void Landmine::explode()
{
	destroyed = true;

	Sound::sound->playSoundEffect(SFX_EXPLOSION, emitter);
	Explosion* explosion = new Explosion((hkTransform*) &(body->getTransform()), owner);

	explosion->doDamage();
	explosion = NULL;
}


void Landmine::trigger()
{
	if (!triggered && activated)
	{
		triggered = true;
		Sound::sound->playSoundEffect(SFX_BEEP, emitter);
	}
}

LandmineListener::LandmineListener(Landmine* l)
{
	landmine = l;
}

void LandmineListener::collisionAddedCallback(const hkpCollisionEvent& ev)
{
	if (!(landmine->triggered) && (landmine->activated) && !(landmine->destroyed)) {
		landmine->trigger();
	}
}
