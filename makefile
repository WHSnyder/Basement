CC = clang++
CFLAGS = -lpthread -Iinclude/glm/glm -Isrc -std=c++17 -Wno-everything
OPENCV = `pkg-config --cflags --libs opencv`
OPENCV_LIBS = $(OPENCV)

 
all: bin/obj.o bin/csg.o bin/scene.o bin/main.o bin/out

clean: 
	rm ./bin/*

bin/obj.o: src/primitives/Obj.cpp src/primitives/Obj.h src/Ray.h src/Scene.h
	$(CC) $(CFLAGS) -c src/primitives/Obj.cpp -o bin/obj.o


bin/csg.o: src/CSG.h src/CSG.cpp src/primitives/Obj.h
	$(CC) $(CFLAGS) -c src/CSG.cpp -o bin/csg.o


bin/scene.o: src/Scene.cpp src/Scene.h src/CSG.h src/Ray.h
	$(CC) $(CFLAGS) -c src/Scene.cpp -o bin/scene.o


bin/main.o: src/Main.cpp src/Scene.h
	$(CC) $(OPENCV_LIBS) $(CFLAGS) -c src/Main.cpp -o bin/main.o


bin/out: bin/main.o bin/scene.o bin/csg.o bin/obj.o
	$(CC) $(OPENCV_LIBS) $(CFLAGS) bin/main.o bin/scene.o bin/csg.o bin/obj.o -o bin/out