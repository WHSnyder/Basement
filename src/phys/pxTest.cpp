#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>

#define NDEBUG

#include <ctype.h>
#include "PxPhysicsAPI.h"



using namespace physx;


int main(int argc, char **argv){

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	auto mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		
	



	mFoundation -> release();

	return 0;	
}