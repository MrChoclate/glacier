#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cmath>
#include "types.h"
#include "delaunay.h"

using namespace std;


DelaunayTriangles::DelaunayTriangles(vector<Point> v) {
}

Point& DelaunayTriangles::succ(const Point &v, const Point &w) {
    auto list = this->adjancyList[v];
    auto it = find(list.begin(), list.end(), w);
    return (it != list.end()) ? *(it++) : *list.begin();
}

Point& DelaunayTriangles::pred(const Point &v, const Point &w) {
    auto list = this->adjancyList[v];
    auto it = find(list.begin(), list.end(), w);
    return (it != list.begin()) ? *(it--) : *list.end();
}

Point& DelaunayTriangles::first(const Point &v) {
}

Point& DelaunayTriangles::getRightMost() {
    return this->rightMost;
}

Point& DelaunayTriangles::getLeftMost() {
    return this->leftMost;
}

void DelaunayTriangles::insert(const Point &a, const Point &b) {
    double x = b.x - a.x;
    double y = b.y - a.y;
    double theta = atan2(y, x) + M_PI;

    // Insert b into a
    auto list = this->adjancyList.at(a);
    std::list<Point>::iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        double theta2 = atan2(i->x - a.x, i->y - a.y) +  + M_PI;
        if(theta < theta2) {
            list.insert(i, b);
            break;
        }
    }
    if(i == list.end())
        list.insert(i, b);


    // Insert a into b
    theta = atan2(-y, -x) + M_PI;
    list = this->adjancyList.at(b);
    for (i = list.begin(); i != list.end(); ++i) {
        double theta2 = atan2(i->x - b.x, i->y - b.y) +  + M_PI;
        if(theta < theta2) {
            list.insert(i, a);
            break;
        }
    }
    if(i == list.end())
        list.insert(i, a);
}

void DelaunayTriangles::del(const Point &a, const Point &b) {
    this->adjancyList[a].remove(b);
    this->adjancyList[b].remove(a);
}


bool qtest(const Point &h, const Point &i, const Point &j, const Point &k) {
}

bool isRightOf(const Point &z, const Segment &seg) {
}

bool isLeftOf(const Point &z, const Segment &seg) {
}

Point& leftEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? a : b;
}

Point& rightEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? b : a;
}

Point& hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

Point& hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

