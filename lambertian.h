#pragma once
#ifndef lambertianH
#define lambertianH

#include"vec3.h"
#include"material.h"
#include"hittable.h"

//choose a random point in sphere and see if it lies inside sphere then return it
vec3 random_in_unit_sphere() {
    vec3 rand_point;
    do {
        rand_point = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
    } while (dot(rand_point, rand_point) >= 1.0);   //in a unit sphere the squared_length has to lie within 1
    return rand_point;                              //i.e x²+y² <= 1, if >1 then return the last point
};

class lambertian : public material {
public:
    vec3 albedo;
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_count& rec,
        vec3& attenuation, ray& scattered_ray) const {

        vec3 target_pixel =  rec.normal + random_in_unit_sphere();// calculate new point away from sphere
        scattered_ray = ray(rec.p, target_pixel); //calculate the rays scattered on the surface in random direction away from sphere
        attenuation = albedo; // returns the color of the lambertian material
        return true;
    
    }
};
#endif // lambertianH
