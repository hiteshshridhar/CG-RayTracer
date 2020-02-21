#pragma once
#ifndef HITTABLELISTH
#define HITTABLELISTH

#include "hittable.h"

class hittableList : public hittable {
public:
    hittableList() {}
    hittableList(hittable **l, int n) { list = l; list_size = n; }
    //~HITTABLELISTH(){}
    virtual bool hit(
        const ray& r, float tmin, float tmax, hit_count& rec) const;
    hittable **list;
    int list_size;
};

bool hittableList::hit(const ray& r, float t_min, float t_max,
    hit_count& rec) const {

    hit_count temp_rec;
    bool got_hit = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            got_hit = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return got_hit;
}

#endif