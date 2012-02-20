#include "Racer.h"

int Racer::xID = 0;
int Racer::yID = 1;
int Racer::zID = 2;

ConfigReader Racer::config = ConfigReader();
	
hkVector4 Racer::xAxis = hkVector4(1.0f, 0.0f, 0.0f);
hkVector4 Racer::yAxis = hkVector4(0.0f, 1.0f, 0.0f);
hkVector4 Racer::zAxis = hkVector4(0.0f, 0.0f, 1.0f);
hkVector4 Racer::attachFL = hkVector4(-1.15f, -0.8f, 1.5f);
hkVector4 Racer::attachFR = hkVector4(1.15f, -0.8f, 1.5f);
hkVector4 Racer::attachRL = hkVector4(-1.15f, -1.0f, -1.5f);
hkVector4 Racer::attachRR = hkVector4(1.15f, -1.0f, -1.5f);

/*hkReal Racer::chassisMass = 1000.0f;
float Racer::accelerationScale = 15.0f;
float Racer::rearSpringK = 26000.0f;
float Racer::frontSpringK = 25000.0f;
float Racer::rearDamperC = 10200.0f;
float Racer::frontDamperC = 10000.0f;*/

hkReal Racer::chassisMass = config.chassisMass;
float Racer::accelerationScale = config.accelerationScale;
float Racer::rearSpringK = config.kRear;
float Racer::frontSpringK = config.kFront;
float Racer::rearDamperC = config.rearDamping;
float Racer::frontDamperC = config.frontDamping;

const float FRONTWHEELYCONSTRAINT = 0.5f;
const float REARWHEELYCONSTRAINT = 0.7f;


Racer::Racer(IDirect3DDevice9* device, Renderer* r, Physics* p, RacerType racerType)
{
	index = -1;

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

	physics = p;


	// Set up filter group (so the car doesn't collide with the wheels)
	int collisionGroupFilter = p->getFilter();
	

	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(1.0f, 0.75f, 2.5f);		//Half extent for racer rigid body box
	info.m_shape = new hkpBoxShape(halfExtent);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
	info.m_restitution = 1.0f;
	
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtent, chassisMass, massProperties);
	massProperties.m_centerOfMass = hkVector4(0.0f, 0.0f, -1.0f);	// Move the centre of mass back a bit
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_AI, collisionGroupFilter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();

	index = r->addDrawable(drawable);
	p->addRigidBody(body);


	// Create tires
	wheelFL = new FrontWheel(device, collisionGroupFilter, config.frontWheelMass);
	r->addDrawable(wheelFL->drawable);
	p->addRigidBody(wheelFL->body);
	
	WheelListener* listenFL = new WheelListener(&(wheelFL->touchingGround));
	wheelFL->body->addContactListener(listenFL);

	
	wheelFR = new FrontWheel(device, collisionGroupFilter, config.frontWheelMass);
	r->addDrawable(wheelFR->drawable);
	p->addRigidBody(wheelFR->body);

	WheelListener* listenFR = new WheelListener(&(wheelFR->touchingGround));
	wheelFL->body->addContactListener(listenFR);
	


	wheelRL = new RearWheel(device, collisionGroupFilter, config.rearWheelMass);
	r->addDrawable(wheelRL->drawable);
	p->addRigidBody(wheelRL->body);

	WheelListener* listenRL = new WheelListener(&(wheelRL->touchingGround));
	wheelFL->body->addContactListener(listenRL);


	wheelRR = new RearWheel(device, collisionGroupFilter, config.rearWheelMass);
	r->addDrawable(wheelRR->drawable);
	p->addRigidBody(wheelRR->body);
	
	WheelListener* listenRR = new WheelListener(&(wheelRR->touchingGround));
	wheelFL->body->addContactListener(listenRR);

	// Now constrain the tires
	hkpGenericConstraintData* constraint;
	hkpConstraintInstance* constraintInst;

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFL, constraint, FRONT);
	constraintInst = new hkpConstraintInstance(wheelFL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFR, constraint, FRONT);
	constraintInst = new hkpConstraintInstance(wheelFR->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();
	
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRL, constraint, REAR);
	constraintInst = new hkpConstraintInstance(wheelRL->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRR, constraint, REAR);
	constraintInst = new hkpConstraintInstance(wheelRR->body, body, constraint);
	p->world->addConstraint(constraintInst);
	constraint->removeReference();

	
	hkpConstraintStabilizationUtil::stabilizeRigidBodyInertia(body);

	reset();
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
		//Ceate structures for raycast from wheel attach pos to ground
		hkpWorldRayCastInput inputs[4];
		hkpClosestRayHitCollector collectors[4];
		hkpWorldRayCastInput input;

		hkVector4 down = drawable->getYhkVector();
		down.mul(-1);
		down.normalize3();

		//Set ray parameters for raycast

		hkVector4 pos = drawable->gethkPosition();
		hkVector4 attachPos = attachFL;
		attachPos.add(pos);
		inputs[0].m_from = attachPos;
		inputs[0].m_to.setAddMul4(attachPos, down, FRONTWHEELYCONSTRAINT+FRONTWHEELRAD);
		attachPos = attachFR;
		attachPos.add(pos);
		inputs[1].m_from = attachPos;
		inputs[1].m_to.setAddMul4(attachPos, down, FRONTWHEELYCONSTRAINT+FRONTWHEELRAD);
		attachPos = attachRL;
		attachPos.add(pos);
		inputs[2].m_from = attachPos;
		inputs[2].m_to.setAddMul4(attachPos, down, REARWHEELYCONSTRAINT+FRONTWHEELRAD);
		attachPos = attachRR;
		attachPos.add(pos);
		inputs[3].m_from = attachPos;
		inputs[3].m_to.setAddMul4(attachPos, down, REARWHEELYCONSTRAINT+FRONTWHEELRAD);

		hkpWorldRayCaster rayCaster;
		rayCaster.castRayGroup(*(physics->world)->getBroadPhase(), inputs, 4, physics->world->getCollisionFilter(), collectors, sizeof(collectors[0]) ); 

		if(collectors[0].hasHit())
		{
			hkVector4 intersect;
			intersect.setInterpolate4(inputs[0].m_from, inputs[0].m_to, collectors[0].getHit().m_hitFraction );

			hkVector4 wheelRad = drawable->getYhkVector();
			wheelRad.normalize3();
			wheelRad.mul(FRONTWHEELRAD);
			intersect.add(wheelRad);

			wheelFL->setPosAndRot(intersect.getComponent(0),intersect.getComponent(1),intersect.getComponent(2),0,0,0);
		}
		/*if(collectors[1].hasHit())
		{
			hkVector4 intersect;
			intersect.setInterpolate4(inputs[1].m_from, inputs[1].m_to, collectors[1].getHit().m_hitFraction );

			hkVector4 wheelRad = drawable->getYhkVector();
			wheelRad.normalize3();
			wheelRad.mul(FRONTWHEELRAD);
			intersect.add(wheelRad);

			wheelFL->setPosAndRot(intersect.getComponent(0),intersect.getComponent(1),intersect.getComponent(2),0,0,0);
		}
		if(collectors[2].hasHit())
		{
			hkVector4 intersect;
			intersect.setInterpolate4(inputs[2].m_from, inputs[2].m_to, collectors[2].getHit().m_hitFraction );

			hkVector4 wheelRad = drawable->getYhkVector();
			wheelRad.normalize3();
			wheelRad.mul(FRONTWHEELRAD);
			intersect.add(wheelRad);

			wheelFL->setPosAndRot(intersect.getComponent(0),intersect.getComponent(1),intersect.getComponent(2),0,0,0);
		}
		if(collectors[3].hasHit())
		{
			hkVector4 intersect;
			intersect.setInterpolate4(inputs[3].m_from, inputs[3].m_to, collectors[3].getHit().m_hitFraction );

			hkVector4 wheelRad = drawable->getYhkVector();
			wheelRad.normalize3();
			wheelRad.mul(FRONTWHEELRAD);
			intersect.add(wheelRad);

			wheelFL->setPosAndRot(intersect.getComponent(0),intersect.getComponent(1),intersect.getComponent(2),0,0,0);
		}*/

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


void Racer::buildConstraint(hkVector4* attachmentPt, hkpGenericConstraintData* constraint, WheelType type)
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

	if (type == FRONT)
	{
		kit->setLinearLimit(yID, -FRONTWHEELYCONSTRAINT, FRONTWHEELYCONSTRAINT);
	}
	else
	{
		kit->setLinearLimit(yID, -REARWHEELYCONSTRAINT, REARWHEELYCONSTRAINT);
	}

	kit->end();

	constraint->setSolvingMethod(hkpConstraintAtom::METHOD_STABILIZED);
}

// between -1.0 and 1.0 (backwards is negative)
void Racer::accelerate(float seconds, float value)
{
	if ((value == 0.0f) || ( !(wheelRL->touchingGround) && !(wheelRR->touchingGround)))
		return;

	hkVector4 forward = drawable->getZhkVector();
	hkVector4 vel = body->getMotion()->getLinearVelocity();
	float dot = vel.dot3(drawable->getZhkVector());

	if ((dot > 0.0f) && (value < 0.0f))
		value *= 2.0f;

	forward.mul(value * chassisMass * accelerationScale);
	

	hkVector4 leftPoint = hkVector4(body->getPosition());
	leftPoint.add4(attachRL);

	hkVector4 rightPoint = hkVector4(body->getPosition());
	rightPoint.add4(attachRR);
	
	if (wheelRL->touchingGround)
		body->applyForce(seconds, forward, leftPoint);

	if (wheelRR->touchingGround)
		body->applyForce(seconds, forward, rightPoint);
}


// between -1.0 and 1.0 (left is negative)
void Racer::steer(float seconds, float value)
{
	currentSteering = value;

	if (!(wheelFL->touchingGround) && !(wheelFR->touchingGround))
		return;

	hkVector4 vel = body->getMotion()->getLinearVelocity();
	float dot = vel.dot3(drawable->getZhkVector());

	bool negative = false;

	if (dot < 0.0f)
	{
		negative = true;
		dot *= -1;
	}
	
	float torqueScale = 0.0f;
	float centripScale = 0.0f;

	// Now adjust forces
	if (dot < 5.0f)
	{
		torqueScale = 25.0f;
		centripScale = 10.0f;
	}
	else if (dot < 10.0f)
	{
		torqueScale = 50.0f;
		centripScale = 30.0f;
	}
	else if (dot < 15.0f)
	{
		torqueScale = 50.0f;
		centripScale = 40.0f;
	}
	else if (dot < 20.0f)
	{
		torqueScale = 60.0f;
		centripScale = 50.0f;
	}
	else if (dot < 30.0f)
	{
		torqueScale = 65.0f;
		centripScale = 50.0f;
	}
	else if (dot < 35.0f)
	{
		torqueScale = 70.0f;
		centripScale = 50.0f;
	}
	else if (dot < 40.0f)
	{
		torqueScale = 75.0f;
		centripScale = 60.0f;
	}
	else if (dot < 45.0f)
	{
		torqueScale = 80.0f;
		centripScale = 70.0f;
	}
	else if (dot < 60.0f)
	{
		torqueScale = 130.0f;
		centripScale = 85.0f;
	}
	else if (dot >= 60.0f)
	{
		torqueScale = 150.0f;
		centripScale = 100.0f;
	}
	else
	{
		torqueScale = 25.0f;
		centripScale = 10.0f;
	}

	if (negative)
	{
		torqueScale *= -1;
		centripScale *= -1;
	}

	hkVector4 force = drawable->getYhkVector();
	force.mul(value * chassisMass * torqueScale);
	body->applyTorque(seconds, force);
	
	force = drawable->getXhkVector();
	force.mul(value * chassisMass * centripScale);
	body->applyForce(seconds, force);
}


void Racer::reset()
{
	hkVector4 reset = hkVector4(0.0f, 0.0f, 0.0f);
	setPosAndRot(0.0f, 10.0f, 0.0f, 0, 0, 0);
	body->setLinearVelocity(reset);
	wheelFL->body->setLinearVelocity(reset);
	wheelFR->body->setLinearVelocity(reset);
	wheelRL->body->setLinearVelocity(reset);
	wheelRR->body->setLinearVelocity(reset);

	//Reread config file
	config.loadFile();
	body->setMass(config.chassisMass);
	wheelFL->body->setMass(config.frontWheelMass);
	wheelFR->body->setMass(config.frontWheelMass);
	wheelRL->body->setMass(config.rearWheelMass);
	wheelRR->body->setMass(config.rearWheelMass);
	accelerationScale = config.accelerationScale;
	rearSpringK = config.kRear;
	frontSpringK = config.kFront;
	rearDamperC = config.rearDamping;
	frontDamperC = config.frontDamping;
}


void Racer::applySprings(float seconds)
{
	// Applies springs and dampers using the helper function getForce()
	hkVector4 force, point;
	D3DXMATRIX transMat;
	(body->getTransform()).get4x4ColumnMajor(transMat);
	hkVector4 upVector = hkVector4(transMat._21, transMat._22, transMat._23);

	if (wheelFL->touchingGround)
	{
		force = getForce(&upVector,  wheelFL->body, &attachFL, FRONT);
		point.setTransformedPos(body->getTransform(), attachFL);
		body->applyForce(seconds, force, point);
	}

	if (wheelFR->touchingGround)
	{
		force = getForce(&upVector,  wheelFR->body, &attachFR, FRONT);
		point.setTransformedPos(body->getTransform(), attachFR);
		body->applyForce(seconds, force, point);
	}

	if (wheelRL->touchingGround)
	{
		force = getForce(&upVector,  wheelRL->body, &attachRL, REAR);
		point.setTransformedPos(body->getTransform(), attachRL);
		body->applyForce(seconds, force, point);
	}

	if (wheelRR->touchingGround)
	{
		force = getForce(&upVector,  wheelRR->body, &attachRR, REAR);
		point.setTransformedPos(body->getTransform(), attachRR);
		body->applyForce(seconds, force, point);
	}
}


hkVector4 Racer::getForce(hkVector4* up, hkpRigidBody* wheel, hkVector4* attach, WheelType type)
{
	hkVector4 actualPos, restPos, force, damperForce, pointVel;
	float displacement, speedOfDisplacement;

	float k, c;

	if (type == FRONT)
	{
		k = frontSpringK;
		c = frontDamperC;
	}
	else
	{
		k = rearSpringK;
		c = rearDamperC;
	}


	actualPos = wheel->getPosition();
	restPos.setTransformedPos(body->getTransform(), *attach);
	actualPos.sub3clobberW(restPos);

	displacement = actualPos.dot3(*up);

	force = hkVector4(*up);
	force.mul(k * displacement);

	body->getPointVelocity(restPos, pointVel);

	speedOfDisplacement = pointVel.dot3(*up);

	damperForce = hkVector4(*up);
	damperForce.mul(c * -speedOfDisplacement);

	force.add3clobberW(damperForce);

	return force;
}


WheelListener::WheelListener(bool* touchingGround)
{
	touching = touchingGround;
}


// Wheel listener callback setup
void WheelListener::collisionAddedCallback(const hkpCollisionEvent& ev)
{
	*touching = true;
}

void WheelListener::collisionRemovedCallback(const hkpCollisionEvent& ev)
{
	*touching = false;
}