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

Point DelaunayTriangles::succ(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);
    auto it = find(list.begin(), list.end(), w);
    return (it != list.end()) ? *(it++) : *list.begin();
}

Point DelaunayTriangles::pred(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);
    auto it = find(list.begin(), list.end(), w);
    return (it != list.begin()) ? *(it--) : *list.end();
}

Point DelaunayTriangles::first(const Point &v) const {
    // FIXME, This is incorrect
    return this->adjancyList.at(v).front();
}

Point DelaunayTriangles::getRightMost() const {
    return this->rightMost;
}

Point DelaunayTriangles::getLeftMost() const {
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
    double x1 = z.x - seg.a.x;
    double x2 = seg.b.x - seg.a.x;

    double y1 = z.y - seg.a.y;
    double y2 = seg.b.y - seg.a.y;

    double determinant = x1 * y2 - x2 * y1;

    return determinant > 0;
}

bool isLeftOf(const Point &z, const Segment &seg) {
    return !isRightOf(z, seg);
}

Point leftEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? seg.a : seg.b;
}

Point rightEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? seg.b : seg.a;
}

Point& hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

Point& hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

