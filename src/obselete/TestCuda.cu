#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <string>

#include <opencv2/opencv.hpp>

#include "Scene.h"
#include "primitives/Obj.h"
#include "CSG.h"


using namespace std;
using namespace glm;
using namespace std::chrono;



string arg_to_string(char* a) { 
    int i; 
    string s = ""; 
    for (i = 0; i < 1000; i++) { 
    	if (a[i] == 0) break;
        s = s + a[i]; 
    } 
    return s;
} 


// Kernel function to add the elements of two arrays
__global__
void add(int n, vec3 *x, vec3 *y, float *z) {

    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    vec3 curx, cury;

    for (int i = index; i < n; i += stride){
    	curx = x[i];
    	cury = y[i];

        z[i] = dot(curx,cury);
    }
}

void add_serial(int n, vec3 *x, vec3 *y, float *z) {

    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    vec3 curx, cury;

    for (int i = index; i < n; i += stride){
    	curx = x[i];
    	cury = y[i];

        z[i] = dot(curx,cury);
    }
}



int main(void) {


	string filename = "/home/will/projects/cpprtx/meshes/torus.obj";

  	Mesh *torus = new Mesh(filename);
  	auto start = high_resolution_clock::now(); 

  	//return 0;

    int N = 1<<10;
    vec3 *x, *y;
    float *z;

    // Allocate Unified Memory – accessible from CPU or GPU
	//cudaMallocManaged(&x, N*sizeof(vec3));
	//cudaMallocManaged(&y, N*sizeof(vec3));
	//cudaMallocManaged(&z, N*sizeof(float));

    x = malloc()

	// initialize x and y arrays on the host
	for (int i = 0; i < N; i++) {
	    x[i] = vec3(1.0,0.0,0.0);
	    y[i] = vec3(1.0,0.0,0.0);
	}

	int blockSize = 8;
	int numBlocks = (N + blockSize - 1) / blockSize;
	add<<<numBlocks, blockSize>>>(N, x, y, z);
	
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();

	// Check for errors (all values should be 3.0f)
	float maxError = 0.0f;
	
	for (int i = 0; i < N; i++){ 
		maxError += z[i];
	}

	std::cout << "Max error: " << maxError << std::endl;
	
	// Free memory
	cudaFree(x);
	cudaFree(y);

	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<milliseconds>(stop - start); 
	cout << "Elapsed: " << duration.count() << endl; 
	  
	return 0;
}