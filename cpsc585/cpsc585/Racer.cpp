#include "Racer.h"
#include "Rocket.h"
#include "Landmine.h"

int Racer::xID = 0;
int Racer::yID = 1;
int Racer::zID = 2;

ConfigReader Racer::config = ConfigReader();
	
hkVector4 Racer::xAxis = hkVector4(1.0f, 0.0f, 0.0f);
hkVector4 Racer::yAxis = hkVector4(0.0f, 1.0f, 0.0f);
hkVector4 Racer::zAxis = hkVector4(0.0f, 0.0f, 1.0f);
hkVector4 Racer::attachFL = hkVector4(-0.8f, -0.67f, 1.65f);
hkVector4 Racer::attachFR = hkVector4(0.8f, -0.67f, 1.65f);
hkVector4 Racer::attachRL = hkVector4(-0.8f, -0.6f, -1.3f);
hkVector4 Racer::attachRR = hkVector4(0.8f, -0.6f, -1.3f);
hkVector4 Racer::attachCannon = hkVector4(0.0f, 0.6f, 2.1f);

hkReal Racer::chassisMass = config.chassisMass;
float Racer::grip = config.grip;
float Racer::accelerationScale = config.accelerationScale;
float Racer::frontSpringK = config.kFront;
float Racer::rearSpringK = config.kRear;
float Racer::frontDamperC = config.frontDamping;
float Racer::rearDamperC = config.rearDamping;

float Racer::frontExtents = config.frontExtents;
float Racer::rearExtents = config.rearExtents;
float Racer::springForceCap = config.springForceCap;

float Racer::topSpeed = config.topSpeed;
float Racer::dragCoeff = chassisMass*accelerationScale/(topSpeed*topSpeed);

bool Racer::inverse = config.inverse;



Racer::Racer(IDirect3DDevice9* device, RacerType racerType)
{
	engineVoice = NULL;
	laserDraw = new Drawable(LASERMODEL, "textures/laser.dds", device);
	Renderer::renderer->addDrawable(laserDraw);

	engineVoice = Sound::sound->reserveSFXVoice();

	health = 100;
	kills = 0;
	suicides = 0;
	deaths = 0;
	givenDamage = 0;
	takenDamage = 0;
	laserReady = true;
	laserTime = 0.0f;

	index = -1;

	currentSteering = 0.0f;
	currentAcceleration = 0.0f;

	lookDir.set(0, 0, 1);
	lookHeight = 0;


	switch (racerType)
	{
	case RACER1:
		drawable = new Drawable(RACER, "textures/racerred.dds", device);
		break;
	case RACER2:
		drawable = new Drawable(RACER, "textures/racerblue.dds", device);
		break;
	case RACER3:
		drawable = new Drawable(RACER, "textures/racerorange.dds", device);
		break;
	case RACER4:
		drawable = new Drawable(RACER, "textures/racergreen.dds", device);
		break;
	case RACER5:
		drawable = new Drawable(RACER, "textures/racerteal.dds", device);
		break;
	case RACER6:
		drawable = new Drawable(RACER, "textures/raceryellow.dds", device);
		break;
	case RACER7:
		drawable = new Drawable(RACER, "textures/racerpurple.dds", device);
		break;
	case RACER8:
		drawable = new Drawable(RACER, "textures/racerpink.dds", device);
		break;
	default:
		drawable = new Drawable(RACER, "textures/racerred.dds", device);
	}


	// Set up filter group (so the car doesn't collide with the wheels)
	int collisionGroupFilter = Physics::physics->getFilter();
	
	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(0.9f, 0.6f, 2.3f);		//Half extent for racer rigid body box
	info.m_shape = new hkpBoxShape(halfExtent);
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
	info.m_centerOfMass.set(0.0f, 0.0f, 0.0f);
	info.m_restitution = 0.0f;
	info.m_maxAngularVelocity = 10.0f;
	info.m_maxLinearVelocity = 170.0f;
	info.m_angularDamping = 0.4f;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtent, chassisMass, massProperties);
	info.setMassProperties(massProperties);
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_AI, collisionGroupFilter);
	body = new hkpRigidBody(info);		//Create rigid body
	body->setLinearVelocity(hkVector4(0, 0, 0));
	info.m_shape->removeReference();

	
	hkpPropertyValue val;
	val.setPtr(this);

	body->setProperty(0, val);

	index = Renderer::renderer->addDrawable(drawable);
	Physics::physics->addRigidBody(body);


	// Create tires
	wheelFL = new FrontWheel(device, collisionGroupFilter);
	Renderer::renderer->addDrawable(wheelFL->drawable);
	Physics::physics->addRigidBody(wheelFL->body);

	
	wheelFR = new FrontWheel(device, collisionGroupFilter);
	Renderer::renderer->addDrawable(wheelFR->drawable);
	Physics::physics->addRigidBody(wheelFR->body);


	wheelRL = new RearWheel(device, collisionGroupFilter);
	Renderer::renderer->addDrawable(wheelRL->drawable);
	Physics::physics->addRigidBody(wheelRL->body);


	wheelRR = new RearWheel(device, collisionGroupFilter);
	Renderer::renderer->addDrawable(wheelRR->drawable);
	Physics::physics->addRigidBody(wheelRR->body);

	// Now constrain the tires
	hkpGenericConstraintData* constraint;
	hkpConstraintInstance* constraintInst;

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFL, constraint, FRONT);
	constraintInst = new hkpConstraintInstance(wheelFL->body, body, constraint);
	Physics::world->addConstraint(constraintInst);
	constraint->removeReference();

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachFR, constraint, FRONT);
	constraintInst = new hkpConstraintInstance(wheelFR->body, body, constraint);
	Physics::world->addConstraint(constraintInst);
	constraint->removeReference();
	
	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRL, constraint, REAR);
	constraintInst = new hkpConstraintInstance(wheelRL->body, body, constraint);
	Physics::world->addConstraint(constraintInst);
	constraint->removeReference();

	constraint = new hkpGenericConstraintData();
	buildConstraint(&attachRR, constraint, REAR);
	constraintInst = new hkpConstraintInstance(wheelRR->body, body, constraint);
	Physics::world->addConstraint(constraintInst);
	constraint->removeReference();

	
	hkpConstraintStabilizationUtil::stabilizeRigidBodyInertia(body);

	reset(&(hkVector4(0, 0, 0, 0)), 0);

	emitter = Sound::sound->getEmitter();

	braking = false;
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

	hkVector4 pos;
	pos.set(posX, posY, posZ);

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);

	wheelFL->setPosAndRot(attachFL(0) + pos(0), attachFL(1) + pos(1), attachFL(2) + pos(2), rotX, rotY, rotZ);
	wheelFR->setPosAndRot(attachFR(0) + pos(0), attachFR(1) + pos(1), attachFR(2) + pos(2), rotX, rotY, rotZ);
	wheelRL->setPosAndRot(attachRL(0) + pos(0), attachRL(1) + pos(1), attachRL(2) + pos(2), rotX, rotY, rotZ);
	wheelRR->setPosAndRot(attachRR(0) + pos(0), attachRR(1) + pos(1), attachRR(2) + pos(2), rotX, rotY, rotZ);
}


void Racer::update()
{
	if (drawable && body)
	{
		D3DXMATRIX transMat;
		(body->getTransform()).get4x4ColumnMajor(transMat);
		drawable->setTransform(&transMat);

		if (laserTime < 0.5f)
		{
			((LaserModel*)(laserDraw->mesh))->drawLaser = false;
		}
		else
		{
			((LaserModel*)(laserDraw->mesh))->drawLaser = true;
			
			laserDraw->setTransform(&transMat);
		}
		
		// Now update wheels

		// rot1 = car rotation, trans1 = car translation
		D3DXMATRIX rot1, trans1;
		D3DXVECTOR3 scale, trans;
		D3DXQUATERNION rot;
		
		hkVector4 currentPos;
		double dist = 0;
		hkVector4 zDir;
		hkTransform wheelTransform;
		hkRotation carRot;

		carRot = body->getTransform().getRotation();


		wheelTransform = wheelRL->body->getTransform();
		wheelTransform.setRotation(carRot);

		if (!braking && wheelRL->touchingGround)
		{
			currentPos.setXYZ(wheelRL->body->getPosition());
			currentPos.sub(wheelRL->lastPos);

			zDir.setXYZ(drawable->getZhkVector());

			dist = currentPos.dot3(zDir);
			dist /= (0.4);
			dist *= D3DX_PI;
			
			dist += wheelRL->rotation;

			if (hkMath::abs(dist) > D3DX_PI)
			{
				if (dist < 0.0)
					dist += 2.0 * D3DX_PI;
				else
					dist -= 2.0 * D3DX_PI;
			}

			wheelRL->lastPos.setXYZ(wheelRL->body->getPosition());
			wheelRL->rotation = dist;
		}
		

		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationX(&transMat, (float) wheelRL->rotation);
		D3DXMatrixTranslation(&trans1, trans.x, trans.y, trans.z);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelRL->drawable->setTransform(&transMat);




		wheelTransform = wheelRR->body->getTransform();
		wheelTransform.setRotation(carRot);

		if (!braking && wheelRR->touchingGround)
		{
			currentPos.setXYZ(wheelRR->body->getPosition());
			currentPos.sub(wheelRR->lastPos);

			zDir.setXYZ(drawable->getZhkVector());

			dist = currentPos.dot3(zDir);
			dist /= (0.4);
			dist *= D3DX_PI;
			
			dist += wheelRR->rotation;

			if (hkMath::abs(dist) > D3DX_PI)
			{
				if (dist < 0.0)
					dist += 2.0 * D3DX_PI;
				else
					dist -= 2.0 * D3DX_PI;
			}

			wheelRR->lastPos.setXYZ(wheelRR->body->getPosition());
			wheelRR->rotation = dist;
		}

		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationX(&transMat, (float) wheelRR->rotation);
		D3DXMatrixTranslation(&trans1, trans.x, trans.y, trans.z);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelRR->drawable->setTransform(&transMat);




		wheelTransform = wheelFL->body->getTransform();
		wheelTransform.setRotation(carRot);
		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationY(&transMat, currentSteering * 1.11f);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		wheelFL->drawable->setTransform(&transMat);

		wheelTransform.setRotation(carRot);

		if (!braking && wheelFL->touchingGround)
		{
			currentPos.setXYZ(wheelFL->body->getPosition());
			currentPos.sub(wheelFL->lastPos);

			zDir.setXYZ(wheelFL->drawable->getZhkVector());

			dist = currentPos.dot3(zDir);
			dist /= (0.35);
			dist *= D3DX_PI;
			
			dist += wheelFL->rotation;

			if (hkMath::abs(dist) > D3DX_PI)
			{
				if (dist < 0.0)
					dist += 2.0 * D3DX_PI;
				else
					dist -= 2.0 * D3DX_PI;
			}

			wheelFL->lastPos.setXYZ(wheelFL->body->getPosition());
			wheelFL->rotation = dist;
		}

		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationYawPitchRoll(&transMat, currentSteering * 1.11f, (float) wheelFL->rotation, 0);
		D3DXMatrixTranslation(&trans1, trans.x, trans.y, trans.z);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelFL->drawable->setTransform(&transMat);

		

		wheelTransform = wheelFR->body->getTransform();
		wheelTransform.setRotation(carRot);
		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationY(&transMat, currentSteering * 1.11f);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		wheelFR->drawable->setTransform(&transMat);

		wheelTransform.setRotation(carRot);

		if (!braking && wheelFR->touchingGround)
		{
			currentPos.setXYZ(wheelFR->body->getPosition());
			currentPos.sub(wheelFR->lastPos);

			zDir.setXYZ(wheelFR->drawable->getZhkVector());

			dist = currentPos.dot3(zDir);
			dist /= (0.35);
			dist *= D3DX_PI;
			
			dist += wheelFR->rotation;

			if (hkMath::abs(dist) > D3DX_PI)
			{
				if (dist < 0.0)
					dist += 2.0 * D3DX_PI;
				else
					dist -= 2.0 * D3DX_PI;
			}

			wheelFR->lastPos.setXYZ(wheelFR->body->getPosition());
			wheelFR->rotation = dist;
		}

		wheelTransform.get4x4ColumnMajor(transMat);

		D3DXMatrixDecompose(&scale, &rot, &trans, &transMat);
		
		D3DXMatrixRotationQuaternion(&rot1, &rot);
		D3DXMatrixRotationYawPitchRoll(&transMat, currentSteering * 1.11f, (float) wheelFR->rotation, 0);
		D3DXMatrixTranslation(&trans1, trans.x, trans.y, trans.z);
		
		D3DXMatrixMultiply(&transMat, &transMat, &rot1);
		D3DXMatrixMultiply(&transMat, &transMat, &trans1);
		wheelFR->drawable->setTransform(&transMat);

		currentAcceleration = 0.0f;



		// Update 3D sound position
		hkVector4 vec;
		vec.setXYZ(lookDir);
		vec.normalize3();
		
		emitter->OrientFront.x = vec(0);
		emitter->OrientFront.y = vec(1);
		emitter->OrientFront.z = vec(2);

		vec.setXYZ(body->getPosition());

		emitter->Position.x = vec(0);
		emitter->Position.y = vec(1);
		emitter->Position.z = vec(2);

		vec.setXYZ(body->getLinearVelocity());

		emitter->Velocity.x = vec(0);
		emitter->Velocity.y = vec(1);
		emitter->Velocity.z = vec(2);
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
	
	hkVector4 wheelAttach;
	wheelAttach.set(0,0,0);

	kit->setPivotA(wheelAttach);
	kit->setPivotB(*attachmentPt);
	kit->setAngularBasisABodyFrame();
	kit->setAngularBasisBBodyFrame();

	kit->constrainLinearDof(xID);
	kit->constrainLinearDof(zID);
	
	kit->constrainAllAngularDof();

	if (type == FRONT)
	{
		kit->setLinearLimit(yID, -frontExtents, frontExtents / 2.0f);
	}
	else
	{
		kit->setLinearLimit(yID, -rearExtents, rearExtents / 2.0f);
	}
	
	kit->end();

	constraint->setSolvingMethod(hkpConstraintAtom::METHOD_STABILIZED);
}


void Racer::brake(float seconds)
{
	// Braking. Apply force to all wheels
	braking = true;

	hkVector4 point, forward;
	hkTransform trans = body->getTransform();

	float accelForce = chassisMass * -accelerationScale;

	accelForce /= 4.0f;

	if (wheelFL->touchingGround)
	{
		forward = wheelFL->body->getLinearVelocity();
		forward.normalize3IfNotZero();
		forward.mul(accelForce);
		point.setTransformedPos(trans, attachFL);
		body->applyForce(seconds, forward, point);
	}

	if (wheelFR->touchingGround)
	{
		forward = wheelFR->body->getLinearVelocity();
		forward.normalize3IfNotZero();
		forward.mul(accelForce);
		point.setTransformedPos(trans, attachFR);
		body->applyForce(seconds, forward, point);
	}

	if (wheelRL->touchingGround)
	{
		forward = wheelRL->body->getLinearVelocity();
		forward.normalize3IfNotZero();
		forward.mul(accelForce);
		point.setTransformedPos(trans, attachRL);
		body->applyForce(seconds, forward, point);
	}

	if (wheelRR->touchingGround)
	{
		forward = wheelRR->body->getLinearVelocity();
		forward.normalize3IfNotZero();
		forward.mul(accelForce);
		point.setTransformedPos(trans, attachRR);
		body->applyForce(seconds, forward, point);
	}
}


// between -1.0 and 1.0 (backwards is negative)
void Racer::accelerate(float seconds, float value)
{
	if ((value == 0.0f) || ( !(wheelRL->touchingGround) && !(wheelRR->touchingGround)))
	{
		currentAcceleration = 0.0f;
		return;
	}

	hkVector4 point;
	hkTransform trans = body->getTransform();

	float accelForce = value * chassisMass * accelerationScale;

	float yComponent = (drawable->getYVector()).y;

	// Cap force, due to grip constraints
	float maxGrip = chassisMass * accelerationScale * grip * yComponent * yComponent;

	if (accelForce >= maxGrip)
	{
		accelForce = maxGrip;
	}

	currentAcceleration = value;

	hkVector4 forward = drawable->getZhkVector();
	hkVector4 vel = body->getLinearVelocity();
	float dot = vel.dot3(forward);

	if ((dot > 0.1f) && (value < 0.0f))
	{
		if (value < -0.9f)
		{
			brake(seconds);
		}
		else
		{
			accelForce /= 4.0f;

			if (wheelFL->touchingGround)
			{
				forward = wheelFL->body->getLinearVelocity();
				forward.normalize3IfNotZero();
				forward.mul(accelForce);
				point.setTransformedPos(trans, attachFL);
				body->applyForce(seconds, forward, point);
			}

			if (wheelFR->touchingGround)
			{
				forward = wheelFR->body->getLinearVelocity();
				forward.normalize3IfNotZero();
				forward.mul(accelForce);
				point.setTransformedPos(trans, attachFR);
				body->applyForce(seconds, forward, point);
			}

			if (wheelRL->touchingGround)
			{
				forward = wheelRL->body->getLinearVelocity();
				forward.normalize3IfNotZero();
				forward.mul(accelForce);
				point.setTransformedPos(trans, attachRL);
				body->applyForce(seconds, forward, point);
			}

			if (wheelRR->touchingGround)
			{
				forward = wheelRR->body->getLinearVelocity();
				forward.normalize3IfNotZero();
				forward.mul(accelForce);
				point.setTransformedPos(trans, attachRR);
				body->applyForce(seconds, forward, point);
			}
		}
	}
	else
	{
		forward.mul(accelForce / 2.0f);

		if (wheelRL->touchingGround)
		{
			point.setTransformedPos(trans, attachRL);
			body->applyForce(seconds, forward, point);
		}

		if (wheelRR->touchingGround)
		{
			point.setTransformedPos(trans, attachRR);
			body->applyForce(seconds, forward, point);
		}
	}
}


// between -1.0 and 1.0 (left is negative)
// Make sure this is called once every frame, even if value = 0
void Racer::steer(float seconds, float value)
{
	currentSteering = value;

	if ((!(wheelFL->touchingGround) && !(wheelFR->touchingGround))
		|| (!(wheelRL->touchingGround) && !(wheelRR->touchingGround)))
		return;
	
	float torqueScale = 0.0f;
	float centripScale = 0.0f;

	hkVector4 vel = body->getLinearVelocity();
	hkVector4 yVec = drawable->getYhkVector();
	hkVector4 currentAngularVelocity = body->getAngularVelocity();


	float dot = vel.dot3(vel);

	bool negative = false;

	if (dot < 0.0f)
	{
		negative = true;
		dot *= -1;
	}



	float currentAngularSpeed = currentAngularVelocity.dot3(yVec);
	float maxAngularSpeed = 5.0f;


	if (dot < 0.1f)
	{
		maxAngularSpeed = 0.0f;
	}
	else if (dot < 8.0f)
	{
		//maxAngularSpeed = 0.4f * dot;
	}

	float desiredAngularSpeed = value * maxAngularSpeed;
	float deltaAngularSpeed = desiredAngularSpeed - currentAngularSpeed;



	
	
	
	// Now adjust forces (will need to tweak these values later)
	if (dot < 2.0f)
	{
		torqueScale = 0.4f;
	}
	else if (dot < 11.0f)	// < ~40km/h. Low speed, no slip
	{
		torqueScale = 1.0f;
		centripScale = 2.0f;
	}
	else if (dot < 15.0f)
	{

		torqueScale = 5.0f;
		centripScale = 5.0f;
	}
	else if (dot < 25.0f)
	{
		torqueScale = 5.0f;
		centripScale = 7.0f;
	}
	else if (dot < 45.0f)
	{
		torqueScale = 7.0f;
		centripScale = 10.0f;
	}
	else
	{
		torqueScale = 7.0f;
		centripScale = 12.0f;
	}
	
	if (negative)
	{
		torqueScale *= -1;
		centripScale *= -1;
	}

	float yComponent = (drawable->getYVector()).y;

	torqueScale *= yComponent * yComponent;
	centripScale *= yComponent * yComponent;


	hkVector4 force;
	force.setXYZ(drawable->getXhkVector());
	
	if (force.dot3(vel) < 0.0f)
	{
		force.mul(deltaAngularSpeed * chassisMass * centripScale);
		body->applyForce(seconds, force, body->getPosition());
	}

	force.setXYZ(yVec);
	force.mul(deltaAngularSpeed * chassisMass * torqueScale);
	body->applyTorque(seconds, force);
}


void Racer::reset(hkVector4* resetPos, float rotation)
{
	hkVector4 reset;
	reset.set(0.0f, 0.0f, 0.0f);

	hkVector4 resetPosition;
	resetPosition.setXYZ(*resetPos);

	setPosAndRot(resetPosition(0), resetPosition(1), resetPosition(2), 0, rotation, 0);
	body->setLinearVelocity(reset);
	wheelFL->body->setLinearVelocity(reset);
	wheelFR->body->setLinearVelocity(reset);
	wheelRL->body->setLinearVelocity(reset);
	wheelRR->body->setLinearVelocity(reset);

	body->setAngularVelocity(reset);
	wheelFL->body->setAngularVelocity(reset);
	wheelFR->body->setAngularVelocity(reset);
	wheelRL->body->setAngularVelocity(reset);
	wheelRR->body->setAngularVelocity(reset);
}


void Racer::applySprings(float seconds)
{
	// Applies springs and dampers using the helper function getForce()
	hkVector4 force, point;
	hkVector4 upVector = drawable->getYhkVector();
	hkTransform transform = body->getTransform();

	if (wheelFL->touchingGround)
	{
		force = getForce(&upVector,  wheelFL->body, &attachFL, FRONT);
		point.setTransformedPos(transform, attachFL);
		body->applyForce(seconds, force, point);
	}

	if (wheelFR->touchingGround)
	{
		force = getForce(&upVector,  wheelFR->body, &attachFR, FRONT);
		point.setTransformedPos(transform, attachFR);
		body->applyForce(seconds, force, point);
	}

	if (wheelRL->touchingGround)
	{
		force = getForce(&upVector,  wheelRL->body, &attachRL, REAR);
		point.setTransformedPos(transform, attachRL);
		body->applyForce(seconds, force, point);
	}

	if (wheelRR->touchingGround)
	{
		force = getForce(&upVector,  wheelRR->body, &attachRR, REAR);
		point.setTransformedPos(transform, attachRR);
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
	actualPos.sub(restPos);

	displacement = actualPos.dot3(*up);

	if (type == FRONT)
	{
		if (displacement < -frontExtents)
		{
			displacement = -frontExtents;
		}
		else if (displacement > frontExtents)
		{
			displacement = frontExtents;
		}
	}
	else
	{
		if (displacement < -rearExtents)
		{
			displacement = -rearExtents;
		}
		else if (displacement > rearExtents)
		{
			displacement = rearExtents;
		}
	}

	force.setXYZ(*up);
	force.mul(k * displacement);

	body->getPointVelocity(restPos, pointVel);

	speedOfDisplacement = pointVel.dot3(*up);

	damperForce.setXYZ(*up);
	damperForce.mul(c * -speedOfDisplacement);

	force.add(damperForce);

	if (force.dot3(force) > springForceCap*springForceCap)
	{
		float forceMultiplier = springForceCap / ((float)force.length3());
		force.mul(forceMultiplier);
	}
	

	return force;
}


void Racer::applyForces(float seconds)
{
	hkVector4 aVel, vel = body->getLinearVelocity();
	float dot = vel.dot3(vel);
	aVel = body->getAngularVelocity();
	float aDot = aVel.dot3(aVel);

	// Only want to be automatically braking if the player
	// isn't trying to move or already moving
	if (((dot > 0.0f) && (dot < 6.0f) && (aDot != 0.0f)) &&
		(currentAcceleration == 0.0f))
	{
		brake(seconds);
	}
	else
	{
		applyFriction(seconds);
	}

	applyTireRaycast();
	applySprings(seconds);
	applyDrag(seconds);
	

	if (laserTime > 0.0f)
	{
		laserTime -= seconds;
	}
}


// Raycasts tires and adjusts their positions
void Racer::applyTireRaycast()
{
	hkVector4 vel;
	vel.setXYZ(body->getLinearVelocity());
	hkVector4 aVel;
	aVel.setXYZ(body->getAngularVelocity());

	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output;
	hkVector4 from;
	hkVector4 to;
	hkVector4 raycastDir = drawable->getYhkVector();
	raycastDir.mul(-1);
	hkTransform transform = body->getTransform();

	hkUint32 collisionFilterInfo = body->getCollisionFilterInfo();

	// Raycast and reposition each tire
	input = hkpWorldRayCastInput();
	output = hkpWorldRayCastOutput();

	// FRONT LEFT TIRE
	from.setTransformedPos(transform, attachFL);
	to.setXYZ(raycastDir);
	to.mul(frontExtents + 0.35f);
	to.add(from);

	from.setXYZ(raycastDir);
	from.mul(-frontExtents * 4.0f);
	from.add(to);
	
	input.m_from.setXYZ(from);
	input.m_to.setXYZ(to);
	input.m_filterInfo = collisionFilterInfo;
	
	Physics::world->castRay(input, output);
	
	if (output.hasHit())
	{
		wheelFL->touchingGround = true;
		to.sub(from);
		to.mul(output.m_hitFraction);
		raycastDir.mul(-0.35f);

		to.add(from);
		to.add(raycastDir);

		wheelFL->body->setPosition(to);
		wheelFL->lastPos.setXYZ(to);
	}
	else
	{
		wheelFL->touchingGround = false;
		to.sub(from);
		raycastDir.mul(-0.35f);

		to.add(from);
		to.add(raycastDir);

		wheelFL->body->setPosition(to);
	}
	
	input = hkpWorldRayCastInput();
	output = hkpWorldRayCastOutput();

	raycastDir = drawable->getYhkVector();
	raycastDir.mul(-1);

	// FRONT RIGHT TIRE
	from.setTransformedPos(transform, attachFR);
	to.setXYZ(raycastDir);
	to.mul(frontExtents + 0.35f);
	to.add(from);

	from.setXYZ(raycastDir);
	from.mul(-frontExtents * 4.0f);
	from.add(to);

	input.m_from.setXYZ(from);
	input.m_to.setXYZ(to);
	input.m_filterInfo = collisionFilterInfo;

	Physics::world->castRay(input, output);

	if (output.hasHit())
	{
		wheelFR->touchingGround = true;
		to.sub(from);
		to.mul(output.m_hitFraction);
		raycastDir.mul(-0.35f);

		to.add(from);
		to.add(raycastDir);

		wheelFR->body->setPosition(to);
		wheelFR->lastPos.setXYZ(to);
	}
	else
	{
		wheelFR->touchingGround = false;
		to.sub(from);
		raycastDir.mul(-0.35f);

		to.add(from);
		to.add(raycastDir);

		wheelFR->body->setPosition(to);
	}

	input = hkpWorldRayCastInput();
	output = hkpWorldRayCastOutput();

	raycastDir = drawable->getYhkVector();
	raycastDir.mul(-1);

	// REAR LEFT TIRE
	from.setTransformedPos(transform, attachRL);
	to.setXYZ(raycastDir);
	to.mul(rearExtents + 0.4f);
	to.add(from);

	from.setXYZ(raycastDir);
	from.mul(-rearExtents * 4.0f);
	from.add(to);

	input.m_from.setXYZ(from);
	input.m_to.setXYZ(to);
	input.m_filterInfo = collisionFilterInfo;

	Physics::world->castRay(input, output);

	if (output.hasHit())
	{
		wheelRL->touchingGround = true;
		to.sub(from);
		to.mul(output.m_hitFraction);
		raycastDir.mul(-0.4f);

		to.add(from);
		to.add(raycastDir);

		wheelRL->body->setPosition(to);
		wheelRL->lastPos.setXYZ(to);
	}
	else
	{
		wheelRL->touchingGround = false;
		to.sub(from);
		raycastDir.mul(-0.4f);

		to.add(from);
		to.add(raycastDir);

		wheelRL->body->setPosition(to);
	}

	input = hkpWorldRayCastInput();
	output = hkpWorldRayCastOutput();

	raycastDir = drawable->getYhkVector();
	raycastDir.mul(-1);

	// REAR RIGHT TIRE
	from.setTransformedPos(transform, attachRR);
	to.setXYZ(raycastDir);
	to.mul(rearExtents + 0.4f);
	to.add(from);

	from.setXYZ(raycastDir);
	from.mul(-rearExtents * 4.0f);
	from.add(to);

	input.m_from.setXYZ(from);
	input.m_to.setXYZ(to);
	input.m_filterInfo = collisionFilterInfo;

	Physics::world->castRay(input, output);

	if (output.hasHit())
	{
		wheelRR->touchingGround = true;
		to.sub(from);
		to.mul(output.m_hitFraction);
		raycastDir.mul(-0.4f);

		to.add(from);
		to.add(raycastDir);

		wheelRR->body->setPosition(to);
		wheelRR->lastPos.setXYZ(to);
	}
	else
	{
		wheelRR->touchingGround = false;
		to.sub(from);
		raycastDir.mul(-0.4f);

		to.add(from);
		to.add(raycastDir);

		wheelRR->body->setPosition(to);
	}
}


void Racer::applyFriction(float seconds)
{
	float yComponent = drawable->getYVector().y;

	if (yComponent < 0.01f)
		return;
	
	// yComponent is multiplied in multiple times so that you can have a strong grip
	// on the ground, but weak greap on walls (this is done in accelerate() and steer() too)
	float xFrictionForce = yComponent * yComponent * grip * accelerationScale * -chassisMass;
	float zFrictionForce = yComponent * yComponent * (grip * 0.02f) * accelerationScale * -chassisMass;

	hkVector4 xForce, zForce, velocity = body->getLinearVelocity();
	velocity.normalize3IfNotZero();

	xForce = drawable->getXhkVector();

	float dot = velocity.dot3(xForce);
	

	xForce.mul(xFrictionForce * dot);
	body->applyForce(seconds, xForce);

	zForce = drawable->getZhkVector();
		
	dot = velocity.dot3(zForce);

	zForce.mul(zFrictionForce * dot);
	body->applyForce(seconds, zForce);
}



void Racer::applyDrag(float seconds)
{
hkVector4 dragForce = body->getLinearVelocity();
float test = dragCoeff;
float speed = dragForce.normalizeWithLength3();
dragForce.mul(-dragCoeff*speed*speed);
body->applyForce(seconds, dragForce);
}


void Racer::fireLaser()
{
	laserTime = 1.0f;

	Sound::sound->playSoundEffect(SFX_LASER, emitter);

	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output = hkpWorldRayCastOutput();
	hkVector4 from;

	hkTransform trans = body->getTransform();
	from.setTransformedPos(trans, attachCannon);

	input = fireWeapon();

	// Check if anything was hit
	if (input.m_userData == -1)
		return;

	Physics::world->castRay(input, output);

	if (output.hasHit())
	{
		hkpRigidBody* hitBody = (hkpRigidBody*) output.m_rootCollidable->getOwner();

		Racer* attacked = (Racer*) (hitBody->getProperty(0)).getPtr();

		if ((attacked != NULL) && (attacked != this))
		{
			hkVector4 raycastDir;
			raycastDir.setXYZ(input.m_to);

			raycastDir.sub(from);
			raycastDir.normalize3();

			hkVector4 force;
			force.setXYZ(raycastDir);

			//force.mul(chassisMass * 10.0f); // Probably don't need this anymore
			
			input.m_to.sub(from);
			input.m_to.mul(output.m_hitFraction);
			input.m_to.add(from);
			
			hitBody->applyPointImpulse(force, input.m_to);

			attacked->applyDamage(this, LASER_DAMAGE);
		}
		else if ((hitBody->getProperty(1)).getPtr())
		{
			Landmine* mine = ((Landmine*)(hitBody->getProperty(1)).getPtr());
			mine->owner = this;

			if (!mine->triggered && !mine->destroyed)
				mine->trigger();
		}
	}
}





// input.m_userData == -1 if nothing was hit
hkpWorldRayCastInput Racer::fireWeapon()
{
	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output;
	hkVector4 from;
	hkVector4 to;
	
	input = hkpWorldRayCastInput();
	output = hkpWorldRayCastOutput();
	input.m_filterInfo = body->getCollisionFilterInfo();
	
	
	// Raycast FROM camera, TO reticule
	from = body->getPosition();
	hkVector4 look;
	look.setXYZ(lookDir);
	look.mul(-7.0f);
	
	from.add(look);
	from(1) = from(1) + 2.0f;

	to.setXYZ(from);

	look.setXYZ(lookDir);
	look.mul(1000.0f);	// Essentially goes on until it hits something
	to.add(look);
	

	input.m_from.setXYZ(from);
	input.m_to.setXYZ(to);

	Physics::world->castRay(input, output);

	if (output.hasHit())
	{
		// Now raycast from racer's cannon to target
		to.sub(from);
		to.mul(output.m_hitFraction * 1.1f);
		to.add(from);

		hkTransform trans = body->getTransform();
		from.setTransformedPos(trans, attachCannon);


		// TO DO:
		// Make sure that this ray is within a permissible range
		// for the racer to fire from
		
		input = hkpWorldRayCastInput();
		input.m_filterInfo = body->getCollisionFilterInfo();
		
		input.m_from.setXYZ(from);
		input.m_to.setXYZ(to);

		return input;
	}

	input.m_userData = -1;

	return input;
}


void Racer::fireRocket()
{
	Sound::sound->playSoundEffect(SFX_ROCKETLAUNCH, emitter);

	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output = hkpWorldRayCastOutput();
	hkVector4 from;

	hkTransform trans = body->getTransform();
	from.setTransformedPos(trans, attachCannon);

	input = fireWeapon();

	hkVector4 to = input.m_to;

	to.sub(from);

	to.normalize3();
	// Change this so rocket is facing rocketDir when launched
					
	
	Rocket* currentRocket = new Rocket(Renderer::device, this);
					
	currentRocket->owner = this;

	hkVector4 rocketPos;
	hkTransform bodyTransform = body->getTransform();

	hkVector4 rocketAttach;
	rocketAttach.setXYZ(Racer::attachCannon);
	rocketAttach(2) = rocketAttach(2) + 0.7f;

	rocketPos.setTransformedPos(bodyTransform, rocketAttach);
	currentRocket->body->setTransform(bodyTransform);
	currentRocket->body->setPosition(rocketPos);

	to.mul(125.0f);
	currentRocket->body->setLinearVelocity(to);
	currentRocket->update(0.0f);

	DynamicObjManager::manager->addObject(currentRocket);
	currentRocket = NULL;
}


void Racer::dropMine()
{
	Sound::sound->playSoundEffect(SFX_DROPMINE, emitter);

	Landmine* currentMine = new Landmine(Renderer::device);
					
	currentMine->owner = this;

	hkVector4 minePos;
	hkTransform bodyTransform = body->getTransform();

	hkVector4 dropPoint;
	dropPoint.set(0, -0.6f, 2.5f);

	minePos.setTransformedPos(bodyTransform, dropPoint);
	currentMine->body->setTransform(bodyTransform);
	currentMine->body->setPosition(minePos);

	DynamicObjManager::manager->addObject(currentMine);
	currentMine = NULL;
}


void Racer::respawn()
{
	hkVector4 deathPos = body->getPosition();
	hkQuaternion deathRot = body->getRotation();

	reset(&(hkVector4(0, 0, 0, 0)), 0);

	deathPos(1) += 5.0f;
	body->setPositionAndRotation(deathPos, deathRot);
}

void Racer::applyDamage(Racer* attacker, int damage)
{
	health -= damage;
	Sound::sound->playSoundEffect(SFX_CRASH, emitter);

	if (health <= 0)
	{
		Sound::sound->playSoundEffect(SFX_SCREAM, emitter);
		Sound::sound->playSoundEffect(SFX_CAREXPLODE, emitter);

		health = 100;
		//respawn();

		if (attacker)
		{
			if (attacker != this)
			{
				attacker->kills += 1;
				deaths += 1;
			}
			else
			{
				// Suicide
				suicides += 1;
			}
		}
	}
	if(attacker != this){
		attacker->givenDamage += damage;
		takenDamage += damage;
	}
}

void Racer::computeRPM()
{
	int gear;
	float rpm;

	float gearRatios[] = {2.97f, 2.97f, 1.8f, 1.43f, 1.0f, 0.84f};

	hkVector4 forward;
	forward.setXYZ(drawable->getZhkVector());
	hkVector4 vel;
	vel.setXYZ(body->getLinearVelocity());

	float forwardSpeed = vel.dot3(forward);

	if (forwardSpeed < 0.0f)
		forwardSpeed *= -1;

	float angularVel = forwardSpeed/0.42f;
	float wheelRPM = angularVel*30.0f/3.14159f;

	gear = (int) (forwardSpeed/15.0f) + 1;
	
	if(gear < 0)
		gear = 0;
	else if (gear > 5)
		gear = 5;

	rpm = gearRatios[gear]*3.14f*wheelRPM;
	rpm /= 2.0f;

	if (rpm < 500.0f)
		rpm = 500.0f;

	Sound::sound->playEngine(emitter, rpm/ 1024.0f, engineVoice);
}
