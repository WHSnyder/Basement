CC = clang++

BASEPATH = /Users/will/projects/cpprtx/
	
OPENCV = `pkg-config --cflags --libs opencv4`
OPENCV_LIBS = $(OPENCV)

PY_INCLUDES = -I/Users/will/projects/py_scripting/pybind11/build/mock_install/include -O3 -I/usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/include/python3.7m -I/usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/include/python3.7m
PY_FLAGS = -shared -undefined dynamic_lookup $(PY_INCLUDES)
FINAL_COMMAND = $(CC) -fPIC -Ilibs/ -Isrc -std=c++17 $(PY_FLAGS) $(OPENCV_LIBS) test_OPENCV.cpp -o test_OPENCV.cpython-37m-darwin.so

all: test


test: 
	$(FINAL_COMMAND)