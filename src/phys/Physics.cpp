#include "phys/Physics.h"

using namespace std;


//For testing the layout pattern of HF data
void print_hf_test(int32_t *raw_hf, PxHeightField *hf, int dim, int x, int z){

	int hraw = raw_hf[x * dim + z];
	float raw_data = (float) (hraw >> 16);
	int height = (int) hf -> getHeight(x,z);

	cout << "Raw value: " << raw_data << " HF value: " << height << endl; 
}


//This may not be necessary, was written when figuring out HFs
PxHeightFieldSample *fill_terrain(int32_t *raw_hf, int dim){

	PxHeightFieldSample *result = new PxHeightFieldSample[dim * dim];

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){

			int index = i * dim + j;
			int height = raw_hf[index] >> 16;
			
			result[index].height = height;
			result[index].materialIndex0 = 3;
			result[index].materialIndex1 = 3;
			result[index].clearTessFlag();
		}
	}

	return result;
}


//Rigid kinematic version, from stackoverflow answer when figuring HFs out
void Simu::addTerrain(int32_t *data, int dim, glm::vec3 scale){

	//float heightscale = scale.y / 32766.0; //max 32 bit signed value, will include neg ranges and adjust transform eventually

	PxTransform trans = PxTransform(PxVec3(-scale.x,0.0f,-scale.z));//-scale.x,-15.0,-scale.z));

	PxHeightFieldSample *samples = fill_terrain(data, dim);

	PxHeightFieldDesc hfDesc = PxHeightFieldDesc();
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = dim;
	hfDesc.nbRows = dim;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField *aHeightField = cook -> createHeightField(hfDesc, gPhysics -> getPhysicsInsertionCallback());
	PxHeightFieldGeometry *hfGeom = new PxHeightFieldGeometry(aHeightField, PxMeshGeometryFlags(), 1.0/3000.0, scale.x * 2.0 / dim, scale.z * 2.0 / dim);//scale.x * 2.0 / dim, scale.z * 2.0 / dim);

	PxRigidDynamic *g_pxHeightField = gPhysics -> createRigidDynamic(trans);
	g_pxHeightField-> setMass(999999999999999.0f); //should probably switch to static sometime soon haha
    g_pxHeightField -> setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

    PxShape* aHeightFieldShape = g_pxHeightField -> createShape(*hfGeom, *gMaterial);
	
	gScene -> addActor(*g_pxHeightField);
}

/*Static version
void Simu::addTerrain(int32_t *data, int rows, int cols, int scale){


	PxTransform trans = PxTransform( PxVec3(0.0,-15.0,0.0) );
	PxRigidStatic *aHeightFieldActor = gPhysics -> createRigidStatic(trans);

	PxHeightFieldSample *samples = fill_terrain(data, rows, cols);

	PxHeightFieldDesc hfDesc = PxHeightFieldDesc();
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = cols;
	hfDesc.nbRows = rows;
	hfDesc.thickness = 4.0;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	//PxHeightField hf = gPhysics -> createHeightField(hfDesc);
	PxHeightField *aHeightField = cook -> createHeightField(hfDesc, gPhysics -> getPhysicsInsertionCallback());
	PxHeightFieldGeometry *hfGeom = new PxHeightFieldGeometry(aHeightField, PxMeshGeometryFlags(), 1.0, 30.0, 30.0);

	PxRigidDynamic *g_pxHeightField = gPhysics -> createRigidDynamic(trans);
	g_pxHeightField-> setMass(400000.0f);

    g_pxHeightField -> setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

    PxShape* aHeightFieldShape = g_pxHeightField -> createShape(*hfGeom, *gMaterial);

	//PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), 1.0, 1.0, 1.0);
	
	//PxShape *aHeightFieldShape = PxRigidActorExt::createExclusiveShape(*aHeightFieldActor, hfGeom, &gMaterial, 1);
	//g_pxHeightField -> attachShape(*aHeightFieldShape);

	print_hf_test(data, aHeightField, 1,2);
	print_hf_test(data, aHeightField, 8,5);
	print_hf_test(data, aHeightField, 5,5);
	print_hf_test(data, aHeightField, 5,8);

	cout << "Is descriptor valid? " << hfDesc.isValid() << endl;

	gScene -> addActor(*g_pxHeightField);
}*/


//Simple transform update for testing purposes
void Simu::getModelMats(){

	PxU32 nbActiveTransforms;
	const PxActiveTransform* activeTransforms = gScene -> getActiveTransforms(nbActiveTransforms);
	PxActiveTransform cur_transform;
	PxMat44 *matTemp;

	for (PxU32 i=0; i < nbActiveTransforms; i++){

		cur_transform = activeTransforms[i];

		glm::mat4 *modelmat = reinterpret_cast<glm::mat4 *>(cur_transform.userData); 
		matTemp = new PxMat44(cur_transform.actor2World);

		memcpy(modelmat, matTemp->front(), 16 * sizeof(float));
		delete(matTemp);//will be changed for eff's sake
	}
}

//Simple collider insertion for testing
void Simu::addSphere(glm::vec3 center, float extent, int tag, void *objptr){

	PxShape* sphere = gPhysics->createShape(PxSphereGeometry(extent), *gMaterial);

	PxTransform localTm(PxVec3(center.x,center.y,center.x));
	PxRigidDynamic *body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*sphere);
	body -> userData = (void *) tag;
	body -> setMass(4.0f);

	body -> userData = objptr;

	gScene -> addActor(*body);
	sphere -> release();
}

//Simple collider insertion for testing
void Simu::addCube(glm::vec3 center, float extent, int tag, void *objptr){

	PxShape* box = gPhysics->createShape(PxBoxGeometry(extent, extent, extent), *gMaterial);

	PxTransform localTm(PxVec3(center.x,center.y,center.x));
	PxRigidDynamic *body = gPhysics -> createRigidDynamic(localTm);
	body -> attachShape(*box);
	body -> userData = (void *) tag;
	body -> setMass(4.0f);

	body -> userData = objptr;
	
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
	
	sceneDesc.gravity = PxVec3(0.0f, -9.0f, 0.0f);
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



#ifdef PYBIND
#include <pybind11/pybind11.h>

namespace py = pybind11;

void setPosition(py::array_<double> newpos, )

PYBIND11_MODULE(Simu, m) {
    py::class_<Simu>(m, "Simulation")
        .def(py::init())
        .def("stepSimu", &Simu::stepSimu);
}
#endif