#pragma once
#ifndef metalH
#define metalH

#include"material.h"

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

class metal : public material {
public:
    metal(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_count& rec,
        vec3& attenuation, ray& scattered_ray) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered_ray = ray(rec.p, reflected);
        attenuation = albedo;
        
        return true;
        //return (dot(scattered_ray.direction(), rec.normal) > 0); 
        //cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length(); /r_in.direction().length()
            //cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));  / r_in.direction().length()
    }
    vec3 albedo;
};
#endif // !metalH