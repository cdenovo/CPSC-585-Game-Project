#include "Racer.h"


Racer::Racer(IDirect3DDevice9* device)
{
	drawable = new Drawable(RACER, "somename", device);
}


Racer::~Racer(void)
{
}

void Racer::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);
}