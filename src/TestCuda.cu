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
void add(int n, float *x, float *y) {
    for (int i = 0; i < n; i++){   
        y[i] = x[i] + y[i];
    }
}



int main(void) {


	string filename = "/home/will/projects/cpprtx/meshes/torus.obj";

  	Mesh *torus = new Mesh(filename);

  	return 0;

    int N = 1<<27;
    float *x, *y;

    // Allocate Unified Memory – accessible from CPU or GPU
	cudaMallocManaged(&x, N*sizeof(float));
	cudaMallocManaged(&y, N*sizeof(float));

	// initialize x and y arrays on the host
	for (int i = 0; i < N; i++) {
	    x[i] = 1.0f;
	    y[i] = 2.0f;
	}

	// Run kernel on 1M elements on the GPU
	add<<<1, 1>>>(N, x, y);

	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();

	// Check for errors (all values should be 3.0f)
	float maxError = 0.0f;
	
	for (int i = 0; i < N; i++){ 
		maxError = fmax(maxError, fabs(y[i]-4.0f));
	    std::cout << "Max error: " << maxError << std::endl;
	}

	// Free memory
	cudaFree(x);
	cudaFree(y);
	  
	return 0;
}