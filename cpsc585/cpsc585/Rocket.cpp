#include "Rocket.h"

IXAudio2SourceVoice* Rocket::rocketVoice = NULL;


Rocket::Rocket(IDirect3DDevice9* device, Racer* o)
{
	drawable = new Drawable(ROCKETMESH, "textures/rocket.dds", device);

	owner = o;
	hkVector4 startAxis;
	startAxis.set(0, 0, -0.5f);

	hkVector4 endAxis;
	endAxis.set(0, 0, 0.5f);

	hkReal radius = 0.2f;

	hkpRigidBodyCinfo info;
	info.m_gravityFactor = 0.0f;
	info.m_friction = 0.0f;
	info.m_shape = new hkpCylinderShape(startAxis, endAxis, radius);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
	info.m_collisionFilterInfo = owner->body->getCollisionFilterInfo();
	
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	body->setAngularVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();
	
	
	listener = new RocketListener(this);
	body->addContactListener(listener);

	Physics::physics->addRigidBody(body);

	destroyed = false;

	emitter = Sound::sound->getEmitter();

	if (!rocketVoice)
		rocketVoice = Sound::sound->reserveSFXVoice();

	owner = NULL;

	lifetime = 6.0f;
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

	hkVector4 pos;
	pos.set(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}

void Rocket::update(float seconds)
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

		Sound::sound->playRocket(emitter, rocketVoice);
		
		lifetime -= seconds;
		
		if (lifetime <= 0.0f)
		{
			destroyed = true;
		}
		else
		{
			Smoke* smoke = new Smoke(Renderer::device);

			hkVector4 pos;
			pos.setXYZ(body->getPosition());
			pos(2) -= 0.7f;

			smoke->setPos(&pos);
			smoke->initialize(Renderer::device, 1.5f);
			DynamicObjManager::manager->addObject(smoke);
			smoke = NULL;
		}
	}
}

void Rocket::explode()
{
	destroyed = true;
	rocketVoice->Stop();

	Sound::sound->playExplosion(emitter);
	Explosion* explosion = new Explosion(Renderer::device, &(body->getTransform()), owner);

	explosion->doDamage();

	DynamicObjManager::manager->addObject(explosion);
	explosion = NULL;
}


RocketListener::RocketListener(Rocket* r)
{
	rocket = r;
}

void RocketListener::collisionAddedCallback(const hkpCollisionEvent& ev)
{
	if (!(rocket->destroyed))
		rocket->explode();
}
