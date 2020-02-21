#pragma once

#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

class material;

struct hit_count
{
    float t; // used to calculate the point of intersection 
    vec3 p; // used for point of intersection
    vec3 normal; // ---a normal vector at the point of intersection
    material* mat_ptr; // ------reference to material of the hit Surface

};

class hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_count& rec) const = 0;
};



#endif
