CC = clang++

ifdef LINUX
	BASEPATH = /home/will/projects/cpprtx/
	GLFLAGS = -Ilibs/glfw/include -L/usr/lib/x86_64-linux-gnu/ -lGL -lGLEW -static -L$(BASEPATH)libs/glfw/build/src -lglfw3
	CFLAGS = -fPIC -lpthread -Ilibs/glm -Ilibs/glm/glm -Isrc -std=c++17 -Wno-everything -Llibs/glm/build/glm -lglm_static
	LDFLAGS = 

else
	BASEPATH = /Users/will/projects/cpprtx/
	GLFLAGS = -Ilibs/glfw/include -L$(BASEPATH)libs/glfw/build/src -lglfw.3 -L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/ -lGL -lGLEW
	CFLAGS = -Os -O2 -fPIC -lpthread -Ilibs/glm -Ilibs/glm/glm -Isrc -std=c++17 -Wno-everything -Llibs/glm/build/ -lglm_static
	LDFLAGS= "-Wl,-rpath,$(BASEPATH)libs/glfw/build/src"
	ASSIMP = -I/Users/will/projects/cpprtx/libs/assimp/include/ -I/Users/will/projects/cpprtx/libs/assimp/build/include/ -lz -L/Users/will/projects/cpprtx/libs/assimp/build/lib/ -lIrrXML -lassimp
	PHYSX = -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include  /Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64/lib*CHECKED.a /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64/lib*CHECKED.a
endif


OPENCV = `pkg-config --cflags --libs opencv`
OPENCV_LIBS = $(OPENCV)


ifndef GLTEST
 
all: bin/obj.o bin/csg.o bin/scene.o bin/main.o bin/out

clean: 
	rm ./bin/*

bin/obj.o: src/primitives/Obj.cpp src/primitives/Obj.h src/Ray.h src/Scene.h
	$(CC) $(CFLAGS) -c src/primitives/Obj.cpp -o bin/obj.o

bin/csg.o: src/CSG.h src/CSG.cpp src/primitives/Obj.h
	$(CC) $(CFLAGS) -c src/CSG.cpp -o bin/csg.o

bin/scene.o: src/Scene.cpp src/Scene.h src/CSG.h src/Ray.h
	$(CC) $(CFLAGS) -c src/Scene.cpp -o bin/scene.o



ifndef CUDA 

bin/main.o: src/Main.cpp src/Scene.h
	$(CC) $(OPENCV_LIBS) $(CFLAGS) -c src/Main.cpp -o bin/main.o

bin/out: bin/main.o bin/scene.o bin/csg.o bin/obj.o
	$(CC) $(OPENCV_LIBS) $(CFLAGS) bin/main.o bin/scene.o bin/csg.o bin/obj.o -o bin/out


else 

bin/main.o: src/TestCuda.cu src/Scene.h
	nvcc $(OPENCV_LIBS) --expt-relaxed-constexpr -lpthread -Ilibs/glm/glm -Isrc -std=c++11 -c src/TestCuda.cu -o bin/main.o

bin/out: bin/main.o bin/scene.o bin/csg.o bin/obj.o
	nvcc $(OPENCV_LIBS) --expt-relaxed-constexpr -lpthread -Ilibs/glm/glm -Isrc -std=c++11 bin/main.o bin/scene.o bin/csg.o bin/obj.o -o bin/out

endif


#Compile basic OpenGL engine
else 


all: bin/ShaderUtils.o bin/Mesh.o bin/glTest.o bin/Physics.o bin/gltst

bin/ShaderUtils.o: src/utils/ShaderUtils.cpp src/utils/ShaderUtils.h
	$(CC) $(CFLAGS) $(GLFLAGS) -c src/utils/ShaderUtils.cpp -o bin/ShaderUtils.o

bin/Mesh.o: src/mesh/Mesh.h src/mesh/Mesh.cpp
	$(CC) $(CFLAGS) $(GLFLAGS) $(ASSIMP) -c src/mesh/Mesh.cpp -o bin/Mesh.o

bin/Physics.o: src/phys/Physics.h src/phys/Physics.cpp
	$(CC) $(CFLAGS) $(PHYSX) -c -o bin/Physics.o src/phys/Physics.cpp

bin/glTest.o: src/glTest.cpp
	$(CC) $(CFLAGS) $(GLFLAGS) -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include -c src/glTest.cpp -o bin/glTest.o

bin/gltst: bin/glTest.o bin/Mesh.o bin/Physics.o
	$(CC) $(CFLAGS) $(GLFLAGS) $(ASSIMP) $(LDFLAGS) $(PHYSX) bin/Physics.o bin/ShaderUtils.o bin/glTest.o bin/Mesh.o -o bin/gltst

clean: 
	rm bin/*

endif