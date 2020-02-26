#include "phys/Physics.h"

using namespace std;

void Simu::getModelMats(float *sphereMat, float *boxMat){

	PxU32 nbActiveTransforms;
	const PxActiveTransform* activeTransforms = gScene -> getActiveTransforms(nbActiveTransforms);
	PxMat44 *matTemp;

	for (PxU32 i=0; i < nbActiveTransforms; i++){

		int id = (int)(size_t) activeTransforms[i].userData;
		
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


void Simu::addSphere(glm::vec3 center, float extent, int tag){

	PxShape* sphere = gPhysics->createShape(PxSphereGeometry(extent), *gMaterial);

	PxTransform localTm(PxVec3(center.x,center.y,center.x));
	PxRigidDynamic *body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*sphere);
	body -> userData = (void *) tag;
	body-> setMass(4.f);
	body-> setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
	gScene -> addActor(*body);

	sphere -> release();
}


void Simu::addCube(glm::vec3 center, float extent, int tag){

	PxShape* box = gPhysics->createShape(PxBoxGeometry(extent, extent, extent), *gMaterial);

	PxTransform localTm(PxVec3(center.x,center.y,center.x));
	PxRigidDynamic *body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*box);
	body -> userData = (void *) tag;
	body -> setMass(4.f);
	body -> setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
	gScene -> addActor(*body);

	box -> release();
}


void Simu::addTerrain(std::int32_t *data, int rows, int cols, int scale){

	PxHeightFieldSample *samples = (PxHeightFieldSample *) malloc(sizeof(PxHeightFieldSample) * (rows * cols));
	PxRigidStatic *aHeightFieldActor = gPhysics -> createRigidStatic(PxTransform( PxVec3(0.0,1.0,0.0) ));

	PxHeightFieldDesc hfDesc;
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = cols;
	hfDesc.nbRows = rows;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField* aHeightField = cook -> createHeightField(hfDesc, gPhysics->getPhysicsInsertionCallback());

	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), 1.0, scale, scale);
	PxShape* aHeightFieldShape = PxRigidActorExt::createExclusiveShape(*aHeightFieldActor, hfGeom, &gMaterial, 1);

	gScene -> addActor(*aHeightFieldActor);
}


void Simu::stepSimu(float timestep){
	PX_UNUSED(true);
	gScene->simulate(timestep);
	gScene->fetchResults(true);
}


void Simu::initSimu(){

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);
	cook = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(gPhysics -> getTolerancesScale()));

	PxSceneDesc sceneDesc(gPhysics -> getTolerancesScale());
	
	sceneDesc.gravity = PxVec3(0.0f, -10.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics -> createScene(sceneDesc);

	gMaterial = gPhysics -> createMaterial(0.5f, 0.5f, 0.6f);

	//PxRigidStatic *groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
	//gScene->addActor(*groundPlane);

	gScene -> setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);

	float halfExtent = 1.0;
	PxShape* sphere = gPhysics->createShape(PxSphereGeometry(halfExtent), *gMaterial);
	PxShape* box = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

	PxTransform localTm( PxVec3(0,28,-1.0) );
	body1 = gPhysics -> createRigidDynamic(localTm);
	body1 -> attachShape(*sphere);
	body1 -> userData = (void *) 1;
	body1 -> setMass(4.f);
	gScene -> addActor(*body1);

	PxTransform localTm2(PxVec3(0,23,-2.0));
	body2 = gPhysics -> createRigidDynamic(localTm2);
	body2 -> attachShape(*box);
	body2 -> userData = (void *) 2;
	body2 -> setMass(4.f);
	gScene -> addActor(*body2);

	box -> release();
	sphere -> release();

	std::cout << "Initialized simu" << std::endl;
}


void Simu::cleanupSimu(){

	PX_UNUSED(true);
	
	gScene -> release();
	gDispatcher -> release();
	gPhysics -> release();
	cook -> release();	
	
	gFoundation -> release();
	
	std::cout << "Simu done" << std::endl;
}