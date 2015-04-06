#ifndef _TYPES_H_
#define _TYPES_H_

struct Point {
    double x, y, x_utm, y_utm;

    friend bool operator<(const Point &a, const Point &b) {
        return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
    }

    friend bool operator==(const Point &a, const Point &b) {
        return (a.x == b.x) && (a.y == b.y);
    }
};
typedef struct Point Point;

struct Segment {
    const Point &a, &b;

    friend bool operator==(const Segment &a, const Segment &b) {
        return (a.a == b.a) && (a.b == b.b);
    }
};
typedef struct Segment Segment;

#endif

