#include "phys/Physics.h"


float *Simu::getModelMats(float **sphereMat, float& **boxMat){

	PxU32 nbActiveTransforms;
	PxActiveTransform* activeTransforms = scene.getActiveTransforms(nbActiveTransforms);

	for (PxU32 i=0; i < nbActiveTransforms; ++i){

    	MyRenderObject* renderObject = static_cast<MyRenderObject*>(activeTransforms[i].userData);
    	renderObject->setTransform(activeTransforms[i].actor2World);
	}

}




void Simu::stepSimu(float timestep){
	PX_UNUSED(false);
	gScene->simulate(timestep);
	gScene->fetchResults(true);
}



void Simu::initSimu(){

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(0);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
	gScene->addActor(*groundPlane);

	float halfExtent = 1.0;
	PxShape* sphere = gPhysics->createShape(PxSphereGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	PxShape* box = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

	PxTransform localTm(PxVec3(0.0,5.0,-5.0,0.0));
	localTm -> userData = (void *) 1;
	PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
	body->attachShape(*sphere);
	body -> userData = (void *) 1;
	PxRigidBodyExt::updateMassAndInertia(*body, 4.0f);
	gScene->addActor(*body);

	PxTransform localTm2(PxVec3(0.0,10.0,-5.0,0.0));
	localTm2 -> userData = (void *) 2;
	body = gPhysics->createRigidDynamic(t.transform(localTm2));
	body->attachShape(*box);
	body -> userData = (void *) 2;
	PxRigidBodyExt::updateMassAndInertia(*body, 4.0f);
	gScene->addActor(*body);

	box -> release();
	sphere -> release();
}
/*
void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for(PxU32 i=0; i<size;i++)
	{
		for(PxU32 j=0;j<size-i;j++)
		{
			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}
*/



void Simu::cleanupSimu()
{
	PX_UNUSED(false);
	
	gScene->release();
	gDispatcher->release();
	gPhysics->release();	
	
	gFoundation->release();
	
	printf("Simu cleaned up.");
}