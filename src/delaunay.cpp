#include <vector>
#include <list>
#include <map>
#include <algorithm>
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
}

void DelaunayTriangles::del(const Point &a, const Point &b) {
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

