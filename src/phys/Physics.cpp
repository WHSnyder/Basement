#include "phys/Physics.h"


void Simu::getModelMats(float *sphereMat, float *boxMat){

	PxU32 nbActiveTransforms;
	const PxActiveTransform* activeTransforms = gScene -> getActiveTransforms(nbActiveTransforms);
	PxMat44 *matTemp;
	//PxActor **buffer = new PxActor*[2];

	//PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//gScene -> getActors(PxActorTypeFlag::eRIGID_DYNAMIC, buffer, 2 * sizeof(PxActor *));

	//std::cout << nbActors << std::endl;

	//matTemp = new PxMat44(((PxRigidActor *) buffer[0]) -> getGlobalPose());
	//memcpy(sphereMat,matTemp->front(),16 * sizeof(float));	
	//delete buffer;
	//delete matTemp;

	//return;

	for (PxU32 i=0; i < nbActiveTransforms; i++){

		int id = (int)(size_t) activeTransforms[i].userData;
		
		//std::cout << activeTransforms[i].userData << std::endl;		

		if (id == 1){

			//sphere
			matTemp = new PxMat44(activeTransforms[i].actor2World);
			memcpy(sphereMat,matTemp->front(),16 * sizeof(float));
			delete(matTemp);
		}
		else if (id == 2){
			//box
			matTemp = new PxMat44(activeTransforms[i].actor2World);
			memcpy(boxMat,matTemp->front(),16 * sizeof(float));
			delete(matTemp);
		}
	}
}




void Simu::stepSimu(float timestep){
	PX_UNUSED(true);
	gScene->simulate(timestep);
	gScene->fetchResults(true);
}



void Simu::initSimu(){

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	
	sceneDesc.gravity = PxVec3(0.0f, -10.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
	gScene->addActor(*groundPlane);

	float halfExtent = 1.0;
	PxShape* sphere = gPhysics->createShape(PxSphereGeometry(halfExtent), *gMaterial);
	PxShape* box = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

	PxTransform localTm(PxVec3(-1.0,6.0,-5.0));
	body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*sphere);
	body -> userData = (void *) 1;
	body->setMass(4.f);
	body->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
	//PxRigidBodyExt::updateMassAndInertia(*body, 4.0f);
	gScene -> addActor(*body);

	PxTransform localTm2(PxVec3(1.0,6.0,-5.0));
	body2 = gPhysics->createRigidDynamic(localTm2);
	body2 -> attachShape(*box);
	body2 -> userData = (void *) 2;
	body2->setMass(4.f);
	body2->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
	//PxRigidBodyExt::updateMassAndInertia(*body2, 4.0f);
	gScene -> addActor(*body2);

	gScene -> setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);

	//box -> release();
	//sphere -> release();

	std::cout << "Initialized simu" << std::endl;
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
	PX_UNUSED(true);
	
	gScene->release();
	gDispatcher->release();
	gPhysics->release();	
	
	gFoundation->release();
	
	std::cout << "Simu done" << std::endl;
}