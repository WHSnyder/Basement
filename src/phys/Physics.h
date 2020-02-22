#include <stdlib.h>

#define NDEBUG

#include <ctype.h>
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

	float *getSphereModelMat();
	float *getBoxModelMat();

	void initSimu();
	void stepSimu(float timestep);
	void cleanupSimu();

	Simu(){
		initSimu();
	}

	~Simu(){
		cleanupSimu();
	}
}
