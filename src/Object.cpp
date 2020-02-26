#include "Object.h"



void GameObject::render(float *view, float *proj){

	shader -> setMats(model_mat, view, proj);
	mesh -> draw(shader -> progID);
}



void GameObject::update(float *model){

	if (!physical){
		model_mat = model;
	}

	else {
		shader -> setMats(model,view,proj);


	}





}


GameObject::GameObject(Mesh *m, int extent, int type, int static_flag, int in_phys, float*data=nullptr){

	physical = in_phys;

	if (physical){
		if (type == 0){

			//box


		}
		else if (type == 1){
			
			//sphere

		}
		else {
			
			//heightfield

		}
	}

}





