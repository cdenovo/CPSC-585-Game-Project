#include "Racer.h"


Racer::Racer(IDirect3DDevice9* device, Renderer* r, Physics* p)
{
	index = -1;

	xID = 0;
	yID = 1;
	zID = 2;

	xAxis = hkVector4(1.0f, 0.0f, 0.0f);
	yAxis = hkVector4(0.0f, 1.0f, 0.0f);
	zAxis = hkVector4(0.0f, 0.0f, 1.0f);



	drawable = new Drawable(RACER, "bricks.dds", device);


	// Set up filter group (so the car doesn't collide with the wheels)
	hkpGroupFilter* filter = new hkpGroupFilter();
	hkpGroupFilterSetup::setupGroupFilter(filter);

	int collisionGroupFilter = filter->getNewSystemGroup();
	p->world->setCollisionFilter(filter);
	filter->removeReference();

	
	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(1.0f, 1.0f, 2.5f);		//Half extent for racer rigid body box
	info.m_shape = new hkpBoxShape(halfExtent);
	info.m_restitution = 0.2f;
	hkReal boxMass = 2000.0f;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtent, boxMass, massProperties);
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_DYNAMIC, collisionGroupFilter);
	body = new hkpRigidBody(info);		//Create rigid body

	info.m_shape->removeReference();
	

	index = r->addDrawable(drawable);
	p->addRigidBody(body);


	// Create tires
	wheelFL = new FrontWheel(device, collisionGroupFilter);
	r->addDrawable(wheelFL->drawable);
	p->addRigidBody(wheelFL->body);

	wheelFR = new FrontWheel(device, collisionGroupFilter);
	r->addDrawable(wheelFR->drawable);
	p->addRigidBody(wheelFR->body);

	wheelRL = new FrontWheel(device, collisionGroupFilter);
	r->addDrawable(wheelRL->drawable);
	p->addRigidBody(wheelRL->body);

	wheelRR = new FrontWheel(device, collisionGroupFilter);
	r->addDrawable(wheelRR->drawable);
	p->addRigidBody(wheelRR->body);


	// Now constrain the tires
	hkVector4 attachmentFL = hkVector4(-1.25f, -0.9f, 1.5f);
	hkpGenericConstraintData* constraint = new hkpGenericConstraintData();
	buildConstraint(&attachmentFL, constraint);
	hkpConstraintInstance* constraintInst = new hkpConstraintInstance(wheelFL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();
	
	hkVector4 attachmentFR = hkVector4(1.25f, -0.9f, 1.5f);
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachmentFR, constraint);
	hkpConstraintInstance* constraintInstFR = new hkpConstraintInstance(wheelFR->body, body, constraint);
	p->world->addConstraint(constraintInstFR);
	constraint->removeReference();
	
	hkVector4 attachmentRL = hkVector4(-1.25f, -0.9f, -1.5f);
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachmentRL, constraint);
	constraintInst = new hkpConstraintInstance(wheelRL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	hkVector4 attachmentRR = hkVector4(1.25f, -0.9f, -1.5f);
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachmentRR, constraint);
	constraintInst = new hkpConstraintInstance(wheelRR->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	

	hkpConstraintStabilizationUtil::stabilizeRigidBodyInertia(body);
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

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotZ));

	hkVector4 pos = hkVector4(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);

	wheelFL->body->setPosition(pos);
	wheelFR->body->setPosition(pos);
	wheelRL->body->setPosition(pos);
	wheelRR->body->setPosition(pos);

	update();
}


void Racer::update()
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);
		
		// Now update wheels
		(wheelFL->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelFL->drawable->setTransform(&transMat);

		(wheelFR->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelFR->drawable->setTransform(&transMat);
		
		(wheelRL->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelRL->drawable->setTransform(&transMat);
		
		(wheelRR->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelRR->drawable->setTransform(&transMat);
	}
}

int Racer::getIndex()
{
	return index;
}


void Racer::buildConstraint(hkVector4* attachmentPt, hkpGenericConstraintData* constraint)
{
	hkpConstraintConstructionKit* kit = new hkpConstraintConstructionKit();
	kit->begin(constraint);
	kit->setLinearDofA(xAxis, xID);
	kit->setLinearDofB(xAxis, xID);
	kit->setLinearDofA(yAxis, yID);
	kit->setLinearDofB(yAxis, yID);
	kit->setLinearDofA(zAxis, zID);
	kit->setLinearDofB(zAxis, zID);

	kit->setPivotA(hkVector4(0,0,0));
	kit->setPivotB(*attachmentPt);
	kit->setAngularBasisABodyFrame();
	kit->setAngularBasisBBodyFrame();

	kit->constrainLinearDof(xID);
	kit->constrainLinearDof(zID);

	kit->constrainToAngularDof(xID);
	//kit->constrainToAngularDof(yID);

	kit->setLinearLimit(yID, -0.25f, 0.25f);
	kit->end();

	constraint->setSolvingMethod(hkpConstraintAtom::METHOD_STABILIZED);
}

// between -1.0 and 1.0 (backwards is negative)
void Racer::accelerate(float seconds, float value)
{
	hkVector4 forward = drawable->getZhkVector();
	

	hkVector4 leftPoint = body->getPosition();
	forward.mul(-1.5f);
	leftPoint.add3clobberW(forward);
	
	forward.mul(value * 50.0f * -1.5f);

	body->applyForce(seconds, forward, leftPoint);
}


// between -1.0 and 1.0 (left is negative)
void Racer::turn(float seconds, float value, bool reversing)
{
	if (reversing)
		value *= -1;

	hkReal angle = body->getRotation().getAngle();

	hkQuaternion quat;
	angle += value * (3.14f/3.75f);

	if (angle >= 3.14f)
		angle -= 2*3.14f;
	else if (angle <= -3.14f)
		angle += 2*3.14f;
	quat = hkQuaternion(drawable->getYhkVector(), angle);
	

	// Actually, should only rotate the "displayable" since the tire height of
	// the physics object is the only part that matters
	wheelFL->body->setRotation(quat);
	wheelFR->body->setRotation(quat);


	hkVector4 force = drawable->getYhkVector();
	force.mul(value * 25000.0f);
	body->applyTorque(seconds, force);
	
	force = drawable->getXhkVector();
	force.mul(value * 1000.0f);
	body->applyForce(seconds, force);
}
