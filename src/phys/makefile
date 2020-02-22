CC = clang++

clean:
	rm pxtest

pxtest: pxTest.cpp 
	clang++ -std=c++17 -o pxtest -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include  /Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64/lib*CHECKED.a pxTest.cpp /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64/lib*CHECKED.a
#-L/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64 -lPhysX3 -lPhysX3Common -lPhysX3Extensions -Wl,-undefined,dynamic_lookup -L/Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64 -lPxFoundation
#-ffunction-sections (compiler switch) AND -Wl,-gc-sections (linker switch) to have the linker "garbage collect" (or remove) unused sections. The -ffunction-sections switch places all functions within their own section. You must use both together to make this work.