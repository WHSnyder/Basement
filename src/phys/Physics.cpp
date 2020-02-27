#include "phys/Physics.h"

using namespace std;



void print_hf_test(int32_t *raw_hf, PxHeightField *hf, int x, int z){

	int hraw = raw_hf[z * 20 + x];
	float raw_data = (float) (hraw >> 16);
	
	//int hf_data = hf[z * 20 + x] ;// x/10.0f, z/10.0f];
	//hf_data = hf_data >> 16;

	int height = (int) hf -> getHeight(x,z);

	cout << "Raw value: " << raw_data << " HF value: " << height << endl; 
}


PxHeightFieldSample *fill_terrain(int32_t *raw_hf, int rows, int cols){

	PxHeightFieldSample *result = new PxHeightFieldSample[rows * cols];

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){

			int height = raw_hf[i * cols + j] >> 16;
			result[i * cols + j].height = height;
			result[i * cols + j].clearTessFlag();
		}
	}

	return result;
}


void Simu::addTerrain(int32_t *data, int rows, int cols, int scale){

	//PxHeightFieldSample *samples = (PxHeightFieldSample *) malloc(sizeof(PxHeightFieldSample) * (rows * cols));
	PxRigidStatic *aHeightFieldActor = gPhysics -> createRigidStatic(PxTransform(PxVec3(0.0,0.0,0.0)));

	PxHeightFieldSample *samples = fill_terrain(data, rows, cols);

    /*this->aHeightField = sdk.createHeightField(hfDesc);
    this->hfGeom = new PxHeightFieldGeometry(aHeightField, PxMeshGeometryFlags(), this->terrain.dy / 255.0, this->terrain.dx, this->terrain.dz);
    this->terrainPos = new PxTransform(PxVec3(-this->terrain.dx*(this->width - 1) / 2, 0.0f, this->terrain.dz*(this->height - 1) / 2), PxQuat(3.1415 / 2.0, PxVec3(0, 1, 0)));
    this->g_pxHeightField = sdk.createRigidDynamic(*this->terrainPos);
    this->g_pxHeightField->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
    PxShape* aHeightFieldShape = this->g_pxHeightField->createShape(*(this->hfGeom), material);*/


	PxHeightFieldDesc hfDesc;// = new PxHeightFieldDesc();
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = cols;
	hfDesc.nbRows = rows;
	hfDesc.thickness = 2.0;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	//PxHeightField hf = gPhysics -> createHeightField(hfDesc);
	//hfGeom = new PxHeightFieldGeometry(aHeightField, PxMeshGeometryFlags(), this->terrain.dy / 255.0, this->terrain.dx, this->terrain.dz);

	PxHeightField *aHeightField = cook -> createHeightField(hfDesc, gPhysics -> getPhysicsInsertionCallback());

	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), 1.0, 1.0, 1.0);
	PxShape *aHeightFieldShape = PxRigidActorExt::createExclusiveShape(*aHeightFieldActor, hfGeom, &gMaterial, 1);


	print_hf_test(data, aHeightField, 0,5);
	print_hf_test(data, aHeightField, 8,8);
	print_hf_test(data, aHeightField, 5,5);
	print_hf_test(data, aHeightField, 5,8);

	cout << "Is descriptor valid? " << hfDesc.isValid() << endl;

	gScene -> addActor(*aHeightFieldActor);
}





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
	body-> setMass(4.0f);
	//body-> setMassSpaceInertiaTensor(PxVec3(10.0f, 10.0f, 10.0f));
	gScene -> addActor(*body);

	sphere -> release();
}


void Simu::addCube(glm::vec3 center, float extent, int tag){

	PxShape* box = gPhysics->createShape(PxBoxGeometry(extent, extent, extent), *gMaterial);

	PxTransform localTm(PxVec3(center.x,center.y,center.x));
	PxRigidDynamic *body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*box);
	body -> userData = (void *) tag;
	body -> setMass(4.0f);
	//body -> setMassSpaceInertiaTensor(PxVec3(10.0f, 10.0f, 10.0f));
	gScene -> addActor(*body);

	box -> release();
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
	gDispatcher = PxDefaultCpuDispatcherCreate(0);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics -> createScene(sceneDesc);

	gMaterial = gPhysics -> createMaterial(0.5f, 0.5f, 0.6f);

    //PxRigidStatic *groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
	//gScene->addActor(*groundPlane);

	gScene -> setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);

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