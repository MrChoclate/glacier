#ifndef _TYPES_H_
#define _TYPES_H_

#include "delaunay.h"

struct Triangle {
    del_point2d_t a, b, c;

    friend bool operator<(const Triangle &a, const Triangle &b) {
        return (a.a.x < b.a.x) || ((a.a.x == b.a.x) && (a.a.y < b.a.y));
    }
};
typedef struct Triangle Triangle;

struct Plan {
    double a, b, c;  //x_utm
    double a_, b_, c_;  //x_utm
};
typedef struct Triangle Triangle;

#endif

