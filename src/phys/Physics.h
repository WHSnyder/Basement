#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#define NDEBUG
#include "PxPhysicsAPI.h"

using namespace physx;


class Simu {

private:

	PxDefaultAllocator gAllocator;
	PxDefaultErrorCallback gErrorCallback;

	PxFoundation *gFoundation = NULL;
	PxPhysics *gPhysics	= NULL;

	PxDefaultCpuDispatcher *gDispatcher = NULL;
	PxScene *gScene	= NULL;

	PxMaterial *gMaterial = NULL;

public:

	//float *getSphereModelMat();
	void getModelMats(float *o1, float *o2);

	void initSimu();
	void stepSimu(float timestep);
	void cleanupSimu();

	Simu(){
		initSimu();
	}

	~Simu(){
		cleanupSimu();
	}
};
