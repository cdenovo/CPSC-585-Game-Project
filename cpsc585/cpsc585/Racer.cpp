#include "Racer.h"


Racer::Racer(IDirect3DDevice9* device, Renderer* r, Physics* p, RacerType racerType)
{
	index = -1;

	xID = 0;
	yID = 1;
	zID = 2;

	xAxis = hkVector4(1.0f, 0.0f, 0.0f);
	yAxis = hkVector4(0.0f, 1.0f, 0.0f);
	zAxis = hkVector4(0.0f, 0.0f, 1.0f);

	attachFL = hkVector4(-1.15f, -0.5f, 1.5f);
	attachFR = hkVector4(1.15f, -0.5f, 1.5f);
	attachRL = hkVector4(-1.15f, -0.5f, -2.0f);
	attachRR = hkVector4(1.15f, -0.5f, -2.0f);

	chassisMass = cReader.chassisMass;

	accelerationScale = cReader.accelerationScale;
	torqueScale = cReader.torqueScale;
	centripScale = cReader.steeringScale;

	currentSteering = 0.0f;


	switch (racerType)
	{
	case PLAYER:
		drawable = new Drawable(RACER, "bricks.dds", device);
		break;
	case AI1:
		drawable = new Drawable(RACER, "checker.dds", device);
		break;
	default:
		drawable = new Drawable(RACER, "bricks.dds", device);
	}

	

	// Set up filter group (so the car doesn't collide with the wheels)
	int collisionGroupFilter = p->getFilter();
	

	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(1.0f, 0.75f, 2.5f);		//Half extent for racer rigid body box
	info.m_shape = new hkpBoxShape(halfExtent);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
	info.m_restitution = 1.0f;
	info.m_gravityFactor = 3.0f;
	info.m_angularDamping = 3.0f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtent, chassisMass, massProperties);
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_AI, collisionGroupFilter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();
	

	index = r->addDrawable(drawable);
	p->addRigidBody(body);


	// Create tires
	wheelFL = new FrontWheel(device, collisionGroupFilter, cReader.frontWheelMass);
	r->addDrawable(wheelFL->drawable);
	p->addRigidBody(wheelFL->body);

	wheelFR = new FrontWheel(device, collisionGroupFilter, cReader.frontWheelMass);
	r->addDrawable(wheelFR->drawable);
	p->addRigidBody(wheelFR->body);

	wheelRL = new RearWheel(device, collisionGroupFilter, cReader.rearWheelMass);
	r->addDrawable(wheelRL->drawable);
	p->addRigidBody(wheelRL->body);

	wheelRR = new RearWheel(device, collisionGroupFilter, cReader.rearWheelMass);
	r->addDrawable(wheelRR->drawable);
	p->addRigidBody(wheelRR->body);


	// Now constrain the tires
	hkpGenericConstraintData* constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFL, constraint);
	hkpConstraintInstance* constraintInst = new hkpConstraintInstance(wheelFL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();
	
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFR, constraint);
	hkpConstraintInstance* constraintInstFR = new hkpConstraintInstance(wheelFR->body, body, constraint);
	p->world->addConstraint(constraintInstFR);
	constraint->removeReference();
	
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRL, constraint);
	constraintInst = new hkpConstraintInstance(wheelRL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRR, constraint);
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
	quat.mul(hkQuaternion(hkVector4(0.0f, 0.0f, 1.0f), rotZ));

	hkVector4 pos = hkVector4(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);

	wheelFL->setPosAndRot(attachFL(0) + pos(0), attachFL(1) + pos(1), attachFL(2) + pos(2), rotX, rotY, rotZ);
	wheelFR->setPosAndRot(attachFR(0) + pos(0), attachFR(1) + pos(1), attachFR(2) + pos(2), rotX, rotY, rotZ);
	wheelRL->setPosAndRot(attachRL(0) + pos(0), attachRL(1) + pos(1), attachRL(2) + pos(2), rotX, rotY, rotZ);
	wheelRR->setPosAndRot(attachRR(0) + pos(0), attachRR(1) + pos(1), attachRR(2) + pos(2), rotX, rotY, rotZ);

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
		(wheelRL->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelRL->drawable->setTransform(&transMat);
		
		(wheelRR->body->getTransform()).get4x4ColumnMajor(transMat);
		wheelRR->drawable->setTransform(&transMat);



		(wheelFL->body->getTransform()).get4x4ColumnMajor(transMat);

		D3DXMATRIX rot1, rot2, trans1;
		D3DXVECTOR3 scale, trans;
		D3DXQUATERNION rot;
		
		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationAxis(&rot2, &(drawable->getYVector()), currentSteering * 1.11f);
		D3DXMatrixTranslation(&trans1, trans.x, trans.y, trans.z);
		
		D3DXMatrixMultiply(&transMat, &rot1, &rot2);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelFL->drawable->setTransform(&transMat);

		(wheelFR->body->getTransform()).get4x4ColumnMajor(transMat);

		D3DXMatrixTranslation(&trans1, transMat._41, transMat._42, transMat._43);

		D3DXMatrixMultiply(&transMat, &rot1, &rot2);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelFR->drawable->setTransform(&transMat);
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

	
	kit->setLinearLimit(yID, -0.75f, -0.1f);
	kit->end();

	constraint->setSolvingMethod(hkpConstraintAtom::METHOD_STABILIZED);
}

// between -1.0 and 1.0 (backwards is negative)
void Racer::accelerate(float seconds, float value)
{
	if (value == 0.0f)
		return;

	hkVector4 forward = drawable->getZhkVector();
	forward.mul(value * chassisMass * accelerationScale);
	
	hkVector4 leftPoint = hkVector4(body->getPosition());
	leftPoint.add4(attachRL);

	hkVector4 rightPoint = hkVector4(body->getPosition());
	rightPoint.add4(attachRR);
	
	body->applyForce(seconds, forward, leftPoint);
	body->applyForce(seconds, forward, rightPoint);
}


// between -1.0 and 1.0 (left is negative)
void Racer::steer(float seconds, float value)
{
	currentSteering = value;

	if (value == 0.0f)
		return;

	currentSteering = value;

	hkVector4 vel = body->getMotion()->getLinearVelocity();

	hkVector4 force = drawable->getYhkVector();
	hkReal dot = vel.dot3(drawable->getZhkVector());
	hkReal denom = vel.length3();
	if (denom != 0)
		dot /= vel.length3();		// This is slow! not good, see note below

	// These values build up too slowly, and shoot up too quickly after a while
	// Really needs to be modified. The slides given to us by Radical suggest
	// building a table of values to use depending on the velocity.


	force.mul(value * chassisMass * torqueScale * dot);
	body->applyTorque(seconds, force);
	
	force = drawable->getXhkVector();
	force.mul(value * chassisMass * centripScale * dot);
	body->applyForce(seconds, force);
}
