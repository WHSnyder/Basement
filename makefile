CC = clang++

BASEPATH = /Users/will/projects/cpprtx/
GLFLAGS = -Ilibs/glfw/include -L$(BASEPATH)libs/glfw/build/src -lglfw.3 -L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/ -lGL -lGLEW
CFLAGS = -fPIC -lpthread -Ilibs/imgui -Ilibs/ -Ilibs/glm -Iinclude -std=c++17 -Wno-everything -Llibs/glm/build/glm -lglm_static
LDFLAGS= "-Wl,-rpath,$(BASEPATH)libs/glfw/build/src"
ASSIMP = -I/Users/will/projects/cpprtx/libs/assimp/include/ -I/Users/will/projects/cpprtx/libs/assimp/build/include/ -lz -L/Users/will/projects/cpprtx/libs/assimp/build/lib/ -lIrrXML -lassimp
PHYSX = -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include  /Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64/lib*CHECKED.a /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64/lib*CHECKED.a
IMGUISOURCES = libs/imgui/bindings/imgui_impl_glfw.cpp libs/imgui/bindings/imgui_impl_opengl3.cpp libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_widgets.cpp

OPENCV = `pkg-config --cflags --libs opencv4`
OPENCV_LIBS = $(OPENCV)

ifdef PY
	PY_INCLUDES = -I/Users/will/projects/py_scripting/pybind11/build/mock_install/include -O3 -I/usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/include/python3.7m -I/usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/include/python3.7m
	PY_FLAGS = -shared -undefined dynamic_lookup $(PY_INCLUDES)
	FINAL_COMMAND = $(CC) $(PY_FLAGS) $(OPENCV_LIBS) $(CFLAGS) $(GLFLAGS) $(ASSIMP) $(LDFLAGS) $(PHYSX) src/rendering/RenderTarget.cpp src/utils/Controls.cpp src/rendering/Shader.cpp src/rendering/Texture.cpp bin/Physics.o bin/ShaderUtils.o bin/glTest.o bin/Mesh.o -o gltest.cpython-37m-darwin.so
else
	FINAL_COMMAND = $(CC) $(OPENCV_LIBS) $(CFLAGS) $(GLFLAGS) $(ASSIMP) $(LDFLAGS) $(PHYSX) $(IMGUISOURCES) src/rendering/RenderTarget.cpp src/utils/controls.cpp src/rendering/Shader.cpp src/rendering/Texture.cpp bin/Physics.o bin/ShaderUtils.o bin/glTest.o bin/Mesh.o -o bin/basement
endif


all: bin/ShaderUtils.o bin/Mesh.o bin/glTest.o bin/Physics.o bin/basement

bin/ShaderUtils.o: src/utils/ShaderUtils.cpp include/utils/ShaderUtils.h
	$(CC) $(CFLAGS) $(GLFLAGS) -c src/utils/ShaderUtils.cpp -o bin/ShaderUtils.o

bin/Mesh.o: include/mesh/Mesh.h src/mesh/Mesh.cpp
	$(CC) $(CFLAGS) $(GLFLAGS) $(ASSIMP) -c src/mesh/Mesh.cpp -o bin/Mesh.o

bin/Physics.o: include/phys/Physics.h src/phys/Physics.cpp
	$(CC) $(CFLAGS) $(PHYSX) -c -o bin/Physics.o src/phys/Physics.cpp

bin/glTest.o: src/GameContext.cpp
	$(CC) $(PY_INCLUDES) $(CFLAGS) $(GLFLAGS) -I/Users/will/projects/cpprtx/libs/physx/PxShared/include -I/Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include -c src/GameContext.cpp -o bin/glTest.o

bin/basement: bin/glTest.o bin/Mesh.o bin/Physics.o
	$(FINAL_COMMAND)

clean: 
	rm bin/*.o 
	rm gltest.cpython-37m-darwin.so