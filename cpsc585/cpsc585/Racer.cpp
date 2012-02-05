#include "Racer.h"


Racer::Racer(IDirect3DDevice9* device)
{
	drawable = new Drawable(RACER, "somename", device);

	hkVector4 halfExtent(.5,.5,.5); //Half extent for racer rigid body box
	hkpBoxShape *box = new hkpBoxShape(halfExtent, 0); //Create shape

	hkpRigidBodyCinfo rigidBodyInfo; //Store shape in rigidBodyInfo
	rigidBodyInfo.m_shape = box;

	body = new hkpRigidBody(rigidBodyInfo); //Create rigid body

	box->removeReference();
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
}