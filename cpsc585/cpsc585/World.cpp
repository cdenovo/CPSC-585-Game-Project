#include "World.h"


World::World(IDirect3DDevice9* device, Renderer* r, Physics* p)
{
	drawable = new Drawable(WORLD, "textures/terrain0.dds", device);

	hkpExtendedMeshShape* meshShape = new hkpExtendedMeshShape();
	meshShape->setRadius(0.0f);

	hkpExtendedMeshShape::TrianglesSubpart subPart;

	subPart.m_vertexBase = (const hkReal*) drawable->mesh->vertices;
	subPart.m_vertexStriding = sizeof(float) * 8;
	subPart.m_numVertices = drawable->mesh->vertexCount;
	subPart.m_indexBase = (const hkReal*) drawable->mesh->indices;
	subPart.m_indexStriding = sizeof(unsigned long) * 3;
	subPart.m_numTriangleShapes = drawable->mesh->indexCount / 3;
	subPart.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;

	meshShape->addTrianglesSubpart(subPart);

	hkpMoppCompilerInput compilerInput;
	hkpMoppCode* moppCode = hkpMoppUtility::buildCode(meshShape, compilerInput);
	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, moppCode);

	hkTransform meshTransform;
	meshTransform.setIdentity();
	hkpMeshWeldingUtility::computeWeldingInfo(meshShape, moppShape, hkpWeldingUtility::WELDING_TYPE_ANTICLOCKWISE);

	hkpRigidBodyCinfo info;
	hkVector4 halfExtent(200.0f, 3.0f, 200.0f);
	info.m_shape = moppShape;
	info.m_motionType = hkpMotion::MOTION_FIXED;	// Static object
	info.m_friction = 2.0f;
	body = new hkpRigidBody(info);
	info.m_shape->removeReference();

	r->addDrawable(drawable);
	p->addRigidBody(body);
}


World::~World(void)
{
	if(body)
	{
		body->removeReference();
	}
}

void World::setPosAndRot(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ)	// In Radians
{
	drawable->setPosAndRot(posX, posY, posZ,
		rotX, rotY, rotZ);

	hkQuaternion quat;
	quat.setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), rotX);
	quat.mul(hkQuaternion(hkVector4(0.0f, 1.0f, 0.0f), rotY));
	quat.mul(hkQuaternion(hkVector4(0.0f, 0.0f, 1.0f), rotZ));

	body->setPositionAndRotation(hkVector4(posX, posY, posZ), quat);
}
