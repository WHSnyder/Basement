#ifndef Object_H
#define Object_H


#include <mesh/Mesh.h>
#include <phys/Physics.h>
#include <gtx/transform.hpp>
#include <perlin/PerlinNoise.hpp>
#include "rendering/Shader.h"



class GameObject {

private:

	static PxPhysics *physics_factory;

public:

	Mesh *mesh;
	PxActor *actor;
	Shader *shader;
	int physical;
	float *model_mat;

	GameObject(Mesh *m, int extent, int type, int static_flag, int in_phys, float *data=nullptr);
	
	void render();
	void update();

}
#endif