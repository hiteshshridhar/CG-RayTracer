#pragma once
#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"
#include "material.h"
//#include"vec3.h"


class sphere : public hittable
{
public:
    sphere() {} // simple sphere constructor
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r) ,mat_ptr(m){}; //copy of constructor
    
    //virtualizing the hit function but not using here
    virtual bool hit(const ray& r, float tmin, float tmax, hit_count& rec) const; 
    vec3 center; // declaring center of sphere
    float radius; // declaring radius of sphere
    material* mat_ptr; //declaring material pointer for the material to handle in sphere
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_count& rec) const {
    
    // calcuting the point of intersection when ray hits the sphere
    
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        // if discriminant is >0, then there can be 2 possible solutions:
        // t can have a positive and a negative value

        float temp = (-b - sqrt(discriminant)) / a; //the formula to solve the quadratic equation
        
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.ray_values(rec.t);
            rec.normal = (rec.p - center) / radius; 
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            
            rec.t = temp;
            rec.p = r.ray_values(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;

        }
    }
    return false;
}

#endif // sphereh
