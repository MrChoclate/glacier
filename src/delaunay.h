#include <vector>
#include <list>
#include <map>
#include "types.h"

using namespace std;

class DelaunayTriangles {
public:
    DelaunayTriangles(vector<Point> v);
    Point succ(const Point &v, const Point &w) const;
    Point pred(const Point &v, const Point &w) const;
    Point getRightMost() const;
    Point getLeftMost() const;
    void insert(const Point &a, const Point &b);
    void del(const Point &a, const Point &b);
    map<Point, list<Point>* > getAdjancyList() const;

    list<Point>::const_iterator ccw_hull(const list<Point>::const_iterator &it) const;
    list<Point>::const_iterator cw_hull(const list<Point>::const_iterator &it) const;
    list<Point>::const_iterator find_hull(const Point &p) const;
private:
    list<Point> hull;
    map<Point, list<Point>* > adjancyList;
    Point rightMost;
    Point leftMost;
};

bool qtest(const Point &h, const Point &i, const Point &j, const Point &k);
bool isRightOf(const Point &z, const Segment &seg);
bool isLeftOf(const Point &z, const Segment &seg);
Point leftEnd(const Segment &seg);
Point rightEnd(const Segment &seg);
Segment hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
Segment hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
map<Point, list<Point>* > merge(const map<Point, list<Point>* > &a, const map<Point, list<Point>* > &b);
list<Point> mergeHull(const Segment &lt, const Segment &up, const list<Point> &leftHull, const list<Point> &rightHull);