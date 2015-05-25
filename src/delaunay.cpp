#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "types.h"
#include "delaunay.h"

//debug
#include <iostream>

using namespace std;


DelaunayTriangles::DelaunayTriangles(vector<Point> v): 
     rightMost(v.back()), leftMost(v.front()) {

    // Base cases
    if(v.size() == 3) {
        Point p1 = v[0];
        Point p2 = v[1];
        Point p3 = v[2];
        adjancyList[p1] = new list<Point>({p2});
        adjancyList[p2] = new list<Point>({p1});
        adjancyList[p3] = new list<Point>({});
        this->insert(p3, p1);
        this->insert(p3, p2);

        double theta2 = atan2(p2.y - p1.y, p2.x - p1.x);
        double theta3 = atan2(p3.y - p1.y, p3.x - p1.x);
        if(theta2 < theta3)
            hull = {p1, p2, p3};
        else
            hull = {p1, p3, p2};

    } else if(v.size() == 2) {
        Point p1 = v.front();
        Point p2 = v.back();
        adjancyList[p1] = new list<Point>({p2});
        adjancyList[p2] = new list<Point>({p1});
        hull = {p1, p2};
    } else {
        // Divide !
        size_t const half_size = v.size() / 2;
        vector<Point> split_lo(v.begin(), v.begin() + half_size);
        vector<Point> split_hi(v.begin() + half_size, v.end());
        DelaunayTriangles lo = DelaunayTriangles(split_lo);
        DelaunayTriangles hi = DelaunayTriangles(split_hi);

        Segment bt = hullLower(lo, hi);
        Segment ut = hullUpper(lo, hi);
        hull = mergeHull(bt, ut, lo.hull, hi.hull);

        cout << bt.a.x << ' ' << bt.a.y << ';' << bt.b.x << ' ' << bt.b.y << endl;
        cout << ut.a.x << ' ' << ut.a.y << ';' << ut.b.x << ' ' << ut.b.y << endl;


        Point l = bt.a;
        Point r = bt.b;
        
        // Conquer !
        adjancyList = merge(lo.getAdjancyList(), hi.getAdjancyList());
        cout << "conquer" << endl;

        for(auto& it: adjancyList) {
            cout << it.first.x << ", " << it.first.y << ":";
            for (auto x = it.second->begin(); x != it.second->end(); ++x) {
                cout << x->x << ", " << x->y << " | ";
            }
            cout << endl;
        }

        while(!(Segment{l, r} == ut)) {
            cout << "l et r" << endl;
            cout << l.x << ' ' << l.y << ';' << r.x << ' ' << r.y << endl;
            bool a = false;
            bool b = false;
            insert(l, r);
            Point r1 = pred(r, l);
            if(isLeftOf(r1, Segment{l, r})) {
                Point r2 = pred(r, r1);
                while(r2 >= hi.getLeftMost() and !qtest(r1, l, r, r2)) {
                    del(r, r1);
                    r1 = r2;
                    r2 = pred(r, r1);

                    if(!isLeftOf(r1, Segment{l, r})) {
                        a = true;
                        break;
                    }
                }
            } else {
                a = true;
            }
            Point l1 = succ(l, r);
            if(isRightOf(l1, Segment{r, l})) {
                Point l2 = succ(l, l1);
                while(l2 <= lo.getRightMost() and !qtest(l, r, l1, l2)) {
                    del(l, l1);
                    l1 = l2;
                    l2 = succ(l, l1);

                    if(!isRightOf(l1, Segment{r, l})) {
                        b = true;
                        break;
                    }
                }
            } else {
                b = true;
            }
            if(a) {
                l = l1;
            } else {
                if(b) {
                    r = r1;
                } else {
                    if(qtest(l, r, r1, l1)) {
                        r = r1;
                    } else {
                        l = l1;
                    }
                }
            }
            assert(!(l == r));
        }
        insert(ut.a, ut.b);
    }

    cout << endl << endl;

    // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }

    // debug
    for(auto it = hull.begin(); it != hull.end(); it++) {
        cout << it->x << ", " << it->y << ";";
    } cout << endl;

}

Point DelaunayTriangles::succ(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);

  /*  // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }
*/
    auto it = find(list->begin(), list->end(), w);
    cout << "succ:" << v.x << "," << v.y << " " << w.x << ',' << w.y << endl;
    assert(it != list->end());
    it++;
    return (it == list->end()) ? list->front() : *(it);
}

Point DelaunayTriangles::pred(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);

/*    // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }
*/
    auto it = find(list->begin(), list->end(), w);
    cout << "pred:" << v.x << ',' << v.y << ' ' << w.x << ',' << w.y << endl;
    assert(it != list->end());
    it = (it == list->begin()) ? list->end() : it;
    it--;
    return *it;
}

Point DelaunayTriangles::getRightMost() const {
    return rightMost;
}

Point DelaunayTriangles::getLeftMost() const {
    return leftMost;
}

double get_theta(const double y, const double x) {
    double theta = atan2(y, x);
    if(theta < 0)
        theta += 2 * M_PI;
    return theta; 
}

void DelaunayTriangles::insert(const Point &a, const Point &b) {
    double x = b.x - a.x;
    double y = b.y - a.y;
    double theta = get_theta(y, x);

    // Insert b into a
    auto list = adjancyList.at(a);
    std::list<Point>::iterator i;
    for (i = list->begin(); i != list->end(); ++i) {
        double theta2 = get_theta(i->y - a.y, i->x - a.x);
        if(theta < theta2) {
            list->insert(i, b);
            break;
        }
    }
    if(i == list->end())
        list->insert(i, b);


    // Insert a into b
    theta = get_theta(-y, -x);
    list = adjancyList.at(b);
    for (i = list->begin(); i != list->end(); ++i) {
        double theta2 = get_theta(i->y - b.y, i->x - b.x);
        if(theta < theta2) {
            list->insert(i, a);
            break;
        }
    }
    if(i == list->end())
        list->insert(i, a);
}

void DelaunayTriangles::del(const Point &a, const Point &b) {
    adjancyList[a]->remove(b);
    adjancyList[b]->remove(a);
}

map<Point, list<Point>* > DelaunayTriangles::getAdjancyList() const {
    return adjancyList;
}

list<Point>::const_iterator DelaunayTriangles::ccw_hull(const list<Point>::const_iterator &it) const {
    auto i = it;
    i++;
    return (i == hull.end()) ? hull.begin() : i;
}

list<Point>::const_iterator DelaunayTriangles::cw_hull(const list<Point>::const_iterator &it) const {
    auto i = it;
    if(i == hull.begin())
        i = hull.end();
    i--;
    return i;
}

list<Point>::const_iterator DelaunayTriangles::find_hull(const Point &p) const {
    return find(hull.begin(), hull.end(), p);
}

double det(int n, double mat[10][10]) {
    double d = 0;
    int c, subi, i, j, subj;
    double submat[10][10];  

    if (n == 2) {
        return( (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
    } else {  
        for(c = 0; c < n; c++) {  
            subi = 0;  
            for(i = 1; i < n; i++) {  
                subj = 0;
                for(j = 0; j < n; j++) {    
                    if (j == c) {
                        continue;
                    }
                    submat[subi][subj] = mat[i][j];
                    subj++;
                }
                subi++;
            }
        d = d + (pow(-1 ,c) * mat[0][c] * det(n - 1 ,submat));
        }
    }
    return d;
}

bool qtest(const Point &h, const Point &i, const Point &j, const Point &k) {
    double mat[10][10] = {{h.x, h.y, h.x*h.x + h.y*h.y, 1}, 
                          {i.x, i.y, i.x*i.x + i.y*i.y, 1},
                          {j.x, j.y, j.x*j.x + j.y*j.y, 1},
                          {k.x, k.y, k.x*k.x + k.y*k.y, 1}};
    return det(4, mat) > 0;
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
    return isRightOf(z, Segment{seg.b, seg.a});
}

Point leftEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? seg.a : seg.b;
}

Point rightEnd(const Segment &seg) {
    return (seg.a.x < seg.b.x) ? seg.b : seg.a;
}

Segment hullLower(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
    Point x = vl.getRightMost();
    Point y = vr.getLeftMost();

    auto ileft = vl.find_hull(x);
    auto iright = vr.find_hull(y);

    ileft = vl.cw_hull(ileft);
    iright = vr.ccw_hull(iright);

    while(1) {
        if(isRightOf(*ileft, Segment{x, y})) {
            x = *ileft;
            ileft = vl.cw_hull(ileft);
        } else if(isRightOf(*iright, Segment{x, y})) {
            y = *iright;
            iright = vr.ccw_hull(iright);
        } else {
            return Segment{x, y};
        }
    }
}

Segment hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
    Point x = vl.getRightMost();
    Point y = vr.getLeftMost();

    auto ileft = vl.find_hull(x);
    auto iright = vr.find_hull(y);

    ileft = vl.ccw_hull(ileft);
    iright = vr.cw_hull(iright);

    while(1) {
        if(isLeftOf(*ileft, Segment{x, y})) {
            x = *ileft;
            ileft = vl.ccw_hull(ileft);
        } else if(isLeftOf(*iright, Segment{x, y})) {
            y = *iright;
            iright = vr.cw_hull(iright);
        } else {
            return Segment{x, y};
        }
    }
}

map<Point, list<Point>* > merge(const map<Point, list<Point>* > &a, const map<Point, list<Point>* > &b) {
    map<Point, list<Point>* > c = a;
    for(auto& i: b) {
        if(a.count(i.first) == 1) {
            // Merge the two lists
            list<Point>* l1 = a.at(i.first);
            list<Point>* l2 = i.second;
            l1->merge(*l2);
            c[i.first] = l1;
        } else {
            c[i.first] = i.second;
        }
    }
    return  c;
}

list<Point> mergeHull(const Segment &lt, const Segment &up, const list<Point> &leftHull, const list<Point> &rightHull) {
    list<Point> new_hull = {};

    auto it = find(rightHull.begin(), rightHull.end(), lt.b);
    while(!(*it == up.b)) {
        new_hull.push_back(*it);
        it++;
        if(it == rightHull.end())
            it = rightHull.begin();
    }
    new_hull.push_back(up.b);

    it = find(leftHull.begin(), leftHull.end(), up.a);
    while(!(*it == lt.a)) {
        new_hull.push_back(*it);
        it++;
        if(it == leftHull.end())
            it = leftHull.begin();
    }
    new_hull.push_back(lt.a);

    return new_hull;
}