#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>

#define NDEBUG

#include <ctype.h>
#include "PxPhysicsAPI.h"

/*class PxDefaultAllocator : public PxAllocatorCallback {
    
    public:
    
	    void* allocate(size_t size, const char*, const char*, int){
	        void* ptr = platformAlignedAlloc(size);
	        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15)==0);
	        return ptr;
	    }
	 
	     void deallocate(void* ptr){
	        platformAlignedFree(ptr);
	     }
};*/

using namespace physx;


int main(int argc, char **argv){

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	//auto mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	
	//if(!mFoundation) fatalError("PxCreateFoundation failed!");

	return 0;	
}