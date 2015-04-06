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
    Point first(const Point &v) const;
    Point getRightMost() const;
    Point getLeftMost() const;
    void insert(const Point &a, const Point &b);
    void del(const Point &a, const Point &b);

private:
    map<Point, list<Point> > adjancyList;
    Point &rightMost;
    Point &leftMost;
};

bool qtest(const Point &h, const Point &i, const Point &j, const Point &k);
bool isRightOf(const Point &z, const Segment &seg);
bool isLeftOf(const Point &z, const Segment &seg);
Point leftEnd(const Segment &seg);
Point rightEnd(const Segment &seg);
Segment hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
Segment hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
