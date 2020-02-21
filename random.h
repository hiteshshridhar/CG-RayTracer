#pragma once
#ifndef RANDOMH
#define RANDOMH
#include "ray.h"
#include <cstdlib>

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
#endif