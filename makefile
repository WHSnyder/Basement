CC = clang++
CFLAGS = -fPIC -lpthread -Iinclude/glm -Isrc -std=c++17 -Wno-everything -Linclude/glm/build -lglm_static
OPENCV = `pkg-config --cflags --libs opencv`
OPENCV_LIBS = $(OPENCV)
GLFLAGS = -Iinclude/glfw/include -Iinclude/glm/glm -Isrc -L/Users/will/projects/cpprtx/include/glfw/build/src -lglfw.3 -framework OpenGL -std=c++17 -Wno-everything
LDFLAGS="-Wl,-rpath,/Users/will/projects/cpprtx/include/glfw/build/src"

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
	nvcc $(OPENCV_LIBS) -Xcompiler "-Wno-everything" -lpthread -Iinclude/glm/glm -Isrc -std=c++11 -c src/TestCuda.cu -o bin/main.o

bin/out: bin/main.o bin/scene.o bin/csg.o bin/obj.o
	nvcc $(OPENCV_LIBS) -Xcompiler "-Wno-everything" -lpthread -Iinclude/glm/glm -Isrc -std=c++11 bin/main.o bin/scene.o bin/csg.o bin/obj.o -o bin/out

endif


#Compile basic OpenGL engine
else 


all: bin/gltst

bin/gltst: bin/glTest.o bin/Mesh.o
	$(CC) -Wno-everything $(GLFLAGS) $(LDFLAGS) bin/glTest.o bin/Mesh.o -o bin/gltst

bin/glTest.o: src/glTest.cpp
	$(CC) -Wno-everything $(CFLAGS) $(GLFLAGS) src/glTest.cpp -o bin/glTest.o

bin/Mesh.o: src/mesh/Mesh.h src/mesh/Mesh.cpp
	$(CC) $(CFLAGS) src/mesh/Mesh.cpp -o bin/Mesh.o

clean: 
	rm bin/gltst

endif