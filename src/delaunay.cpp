#include <vector>
#include <list>
#include <map>
#include "types.h"

using namespace std;


DelaunayTriangles::DelaunayTriangles(vector<Point> v) {
}

Point& DelaunayTriangles::succ(const Point &v, const Point &w) {
}

Point& DelaunayTriangles::pred(const Point &v, const Point &w) {
}

Point& DelaunayTriangles::first(const Point &v) {
}

Point& DelaunayTriangles::getRightMost() {
}

Point& DelaunayTriangles::getLeftMost() {
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
}

Point& rightEnd(const Segment &seg) {
}

Point& hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

Point& hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
}

