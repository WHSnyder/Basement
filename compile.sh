clang++ -Iinclude $(pkg-config --cflags --libs opencv4)  -std=c++11 ./src/Main.cpp ./src/Ray.cpp ./src/Sphere.cpp -o ./bin/tst