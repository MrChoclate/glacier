#include <vector>
#include <list>
#include <map>
#include "types.h"

using namespace std;

class DelaunayTriangles {
public:
    DelaunayTriangles(vector<Point> v);
    Point& succ(const Point &v, const Point &w);
    Point& pred(const Point &v, const Point &w);
    Point& first(const Point &v);
    Point& getRightMost();
    Point& getLeftMost();
    void insert(const Point &a, const Point &b);
    void del(const Point &a, const Point &b);

private:
    map<Point, list<Point> > ajdancyList;
    Point &rightMost;
    Point &leftMost;
};

bool qtest(const Point &h, const Point &i, const Point &j, const Point &k);
bool isRightOf(const Point &z, const Segment &seg);
bool isLeftOf(const Point &z, const Segment &seg);
Point& leftEnd(const Segment &seg);
Point& rightEnd(const Segment &seg);
Point& hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
Point& hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr);
