CC = clang++

clean:
	rm pxtest

pxtest: pxTest.cpp 
	clang++ -o pxtest -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include -L/Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64 -lPxFoundation -L/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64 -lPhysX3 -lPhysX3Common -lPhysX3Extensions pxTest.cpp