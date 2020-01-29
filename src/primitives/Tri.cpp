#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Tri.h"

#include <iostream>

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

using namespace glm;
using namespace std;

/*cv::Vec3b Tri::shade(RayHit *rh, cv::Mat *tex, Obj *objects[], Light *lights[]){

    vec2 uv_hit = *rh -> uv;
    vec2 uv = uv_hit.u * p0 -> uv + uv_hit.v * p1 -> uv + (1 - uv_hit.u - uv_hit.v) * p2 -> uv;

    int u = (int) (tex->rows - 1) * uv.x;
    int v = (int) (tax->cols - 1) * uv.y;

    return Vec3b(0,0,0);//tableimg.at<cv::Vec3b>(u,v);
}*/

cv::Vec3b Tri::shade(RayHit *rhit, cv::Mat *tex, Scene *scene){

    cv::Vec3b col = cv::Vec3b(50,50,50);
    
    int i = -1;

    Ray reflection = Ray(*rhit->entrance,reflect(*rhit->ent_normal, rhit->ray->dir));
    RayHit *reflect_hit = scene -> intersect_scene(reflection, &i);

    if (reflect_hit == nullptr) return col;

    col = scene -> objects[i] -> shade(reflect_hit, tex, scene);

    delete reflect_hit;

    return col;
}
 
RayHit *Tri::intersect_ray(Ray& r) {

    vec3 v0 = p0->coord, v1 = p1->coord, v2 = p2->coord;
    vec3 v0v1 = v1 - v0, v0v2 = v2 - v0; 
    vec3 zvec = normalize(cross(v0v1,v0v2));

    float denom = dot(zvec,r.dir); 

    if (denom > 0.001) { 

        float t = (dot(zvec,v0) - dot(zvec,r.origin)) / denom;

        if (t < 0) return nullptr;

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - v0;

        float d00 = dot(v0v1, v0v1);
        float d01 = dot(v0v1, v0v2);
        float d11 = dot(v0v2, v0v2);
        float d20 = dot(fromOrg, v0v1);
        float d21 = dot(fromOrg, v0v2);
        denom = d00 * d11 - d01 * d01;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        if (u > 1.0 || v > 1.0 || w > 1.0 || u < 0.0 || v < 0.0 || w < 0.0){
            delete hit;
            return nullptr;
        }

        return new RayHit(hit, new vec3(zvec), t, &r);
    } 
    return nullptr; 
}