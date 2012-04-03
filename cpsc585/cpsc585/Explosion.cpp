#include "Explosion.h"
#include "Landmine.h"

Explosion::Explosion(IDirect3DDevice9* device, const hkTransform* trans, Racer* owns)
{
	transform = (hkTransform*) trans;
	owner = owns;

	drawable = new Drawable(RACER, "textures/rocket.dds", device);
	
	D3DXMATRIX transMat;
	trans->get4x4ColumnMajor(transMat);
	drawable->setTransform(&transMat);

	destroyed = false;
	
	lifetime = 2.0f;
}


Explosion::~Explosion(void)
{
	if (drawable)
	{
		delete drawable;
	}
}


void Explosion::doDamage()
{
	hkpShape* blastShape =  new hkpSphereShape(BLAST_RADIUS);
	hkpShapePhantom* blastPhantom = new hkpSimpleShapePhantom(blastShape, *transform);

	hkpCollisionInput inp = *(Physics::world->getCollisionInput());
	inp.setTolerance(BLAST_RADIUS);

	hkpAllCdBodyPairCollector collector;
	
	Physics::world->getPenetrations(blastPhantom->getCollidable(), inp, collector);

	hkArray<hkpRootCdBodyPair> hits;
	hits = collector.getHits();
	
	hkpRigidBody* body;
	Racer* racer;

	hkArrayBase<hkpRootCdBodyPair>::iterator iter = hits.begin();

	hkVector4 pos, racerPos;
	pos.set(0, 0, 0);
	pos.setTransformedPos(*transform, pos);
	double distance;
	int damage;

	// Now deal damage based on distance from centre of explosion
	while (iter < hits.end())
	{
		if (((hkpRigidBody*) iter->m_rootCollidableB->getOwner())->getProperty(0).getPtr())
		{
			body = (hkpRigidBody*) iter->m_rootCollidableB->getOwner();
			racer = (Racer*) body->getProperty(0).getPtr();

			racerPos.setXYZ(body->getPosition());

			racerPos.sub(pos);

			distance = racerPos.length3();
			distance += 1.0f;
		
			damage = (int) hkMath::floor(BLAST_DAMAGE * BLAST_RADIUS / distance);

			racer->applyDamage(owner, damage);

			racerPos.normalize3();
			racerPos.mul((const hkReal) (10.0 * racer->chassisMass * BLAST_RADIUS / distance));
			body->applyLinearImpulse(racerPos);
		}
		else if (((hkpRigidBody*) iter->m_rootCollidableB->getOwner())->getProperty(1).getPtr())
		{
			body = (hkpRigidBody*) iter->m_rootCollidableB->getOwner();
			Landmine* mine;
			mine = (Landmine*) body->getProperty(1).getPtr();

			if (!mine->destroyed)
				mine->explode();
		}

		iter++;
	}
}


void Explosion::update(float seconds)
{
	if (drawable)
	{
		lifetime -= seconds;

		if (lifetime <= 0.0f)
			destroyed = true;
	}
}
