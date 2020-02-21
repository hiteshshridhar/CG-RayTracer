#pragma once

#ifndef MaterialH
#define MaterialH

#include "ray.h"
#include"hittable.h"
#include "random.h"


class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_count& rec, vec3& attenuation,       //==> r_in: incoming ray, scattered: outgoing ray,
        ray& scattered) const = 0;                                      //==> attenuation: how much ray should be absorbed            
};                                                                    //==> hit_count ==> info where the ray hits the surface of what material.    

// ** Code block to represent the refraction ** //

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v); //used for the calculation
    float dt = dot(uv, n); //used to calculate from the cosine value |uv||n|(cos theta)

    //********* to check if the refraction has happened *************//

    float radical = 1.0 - (ni_over_nt * ni_over_nt * (1 - dt * dt));  // if dis. < 0 then we have "no refraction", it is total reflection
    if (radical > 0) {                                              // if dis. > 0 then we have "refraction"    
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(radical); // if dis = 0 then we have total internal reflectiom
        return true;
    }
    else
        return false;
}

// defining reflection 
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

//choose a random point in sphere and see if it lies inside sphere then return it
vec3 random_value() {
    vec3 rand_point;
    do {
        rand_point = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
    } while (dot(rand_point, rand_point) >= 1.0);   //in a unit sphere the squared_length has to lie within 1
    return rand_point;                              //i.e x²+y² <= 1, if >1 then return the last point
};

class metal : public material {
public:
    metal(const vec3& a, float fuzzy) : albedo(a) {
        if (fuzzy < 1) fuzz = fuzzy; else fuzz = 1;
    }
    virtual bool scatter(const ray& r_in, const hit_count& rec,
        vec3& attenuation, ray& scattered_ray) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered_ray = ray(rec.p, reflected + fuzz * random_value());
        attenuation = albedo;

        //return true;
        return (dot(scattered_ray.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};


// WHAT IF BOTH REFLECTION AND REFRACTION HAPPENS // FRESNAL EQUATION DESCRIBES RATIO OF REFLECTION AND TRANSMISSION OF LIGHT

float schlick(float cosine, float ref_idx) {        // Specular reflection can be defined by the following formula:
    float r0 = (1 - ref_idx) / (1 + ref_idx);       //** ref_index = (n1/n2)**  
    r0 = r0 * r0;                                   //----- specular reflection coefficient R0= ((n1-n2)/(n1+n2))²  -------//    
    return r0 + (1 - r0) * pow((1 - cosine), 5);    //R(theta1)=R{0}+(1-R{0})(1- cos(theta1) )^5  --> schlick's approximation //
}

class dielectric : public material {
public:
    
    dielectric(float ri) : ref_idx(ri) {}
    
    virtual bool scatter(const ray& r_in, const hit_count& rec, vec3& attenuation, ray& scattered) const {
        
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal); // reflection
        float ni_over_nt; //refractíve index
        
        attenuation = vec3(1.0, 1.0, 1.0); 
        
        vec3 refracted; //if refraction happens
        float reflect_prob; //reflective coefficient//
        float cosine; //...for calculating the schlicks equation...//

        // -- When the ray shoots from the object back into vaccum (ref_index= n1/n2)   O---> //
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;  //- revert the surface normal //
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal);
            
        }

        //--- When the ray shoots into object --->O , also ref_index= n2/n1
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal);
        }

        //-- When refracted Ray exists then equate the Schlick's value otherwise it is not refracted -- //
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx); //reflective_coefficient

        else
            reflect_prob = 1.0;  //------ Not refracted-----//


        // check with random number: if random_number is < reflect_prob then it is reflected //
        if (random_double() < reflect_prob)
            scattered = ray(rec.p, reflected);

        // check with random number: if random_number is > reflect_prob then it is refracted //
        else
            scattered = ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
};




#endif

