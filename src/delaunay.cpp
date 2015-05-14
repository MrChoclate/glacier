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
    } else if(v.size() == 2) {
        Point p1 = v.front();
        Point p2 = v.back();
        adjancyList[p1] = new list<Point>({p2});
        adjancyList[p2] = new list<Point>({p1});
    } else {
        assert(v.size() != 1);
        // Divide !
        size_t const half_size = v.size() / 2;
        vector<Point> split_lo(v.begin(), v.begin() + half_size);
        vector<Point> split_hi(v.begin() + half_size, v.end());
        DelaunayTriangles lo = DelaunayTriangles(split_lo);
        DelaunayTriangles hi = DelaunayTriangles(split_hi);

        Segment bt = hullLower(lo, hi);
        Segment ut = hullUpper(lo, hi);

        cout << bt.a.x << ' ' << bt.a.y << ';' << bt.b.x << ' ' << bt.b.y << endl;
        cout << ut.a.x << ' ' << ut.a.y << ';' << ut.b.x << ' ' << ut.b.y << endl;


        Point l = (bt.a < bt.b) ? bt.a : bt.b;
        Point r = (bt.a < bt.b) ? bt.b : bt.a;
        
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
            bool a = false;
            bool b = false;
            insert(l, r);
            Point r1 = pred(r, l);
            if(isLeftOf(r1, Segment{l, r})) {
                Point r2 = pred(r, r1);
                while(!qtest(r1, l, r, r2)) {
                    del(r, r1);
                    r1 = r2;
                    r2 = pred(r, r1);
                }
            } else {
                a = true;
            }
            Point l1 = succ(l, r);
            if(isRightOf(l1, Segment{r, l})) {
                Point l2 = succ(l, l1);
                while(!qtest(l, r, l1, l2)) {
                    del(l, l1);
                    l1 = l2;
                    l2 = succ(l, l1);
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
        }
    }

    // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }

}

Point DelaunayTriangles::succ(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);

    // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }

    auto it = find(list->begin(), list->end(), w);
    cout << "succ:" << v.x << "," << v.y << " " << w.x << ',' << w.y << endl;
    assert(it != list->end());
    return (*it == list->back()) ? list->front() : *(it++);
}

Point DelaunayTriangles::pred(const Point &v, const Point &w) const {
    auto list = this->adjancyList.at(v);

    // debug
    for(auto& it: adjancyList) {
        cout << it.first.x << ", " << it.first.y << ":";
        for (auto x = it.second->begin(); x != it.second->end(); ++x) {
            cout << x->x << ", " << x->y << " | ";
        }
        cout << endl;
    }

    auto it = find(list->begin(), list->end(), w);
    cout << "pred:" << v.x << ',' << v.y << ' ' << w.x << ',' << w.y << endl;
    assert(it != list->end());
    return (it == list->begin()) ? list->back() : *(it--);
}

Point DelaunayTriangles::first(const Point &v) const {
    Point begin = adjancyList.at(v)->front();
    Point end = adjancyList.at(v)->back();

    return (isRightOf(v, Segment{end, begin})) ? begin : end;
}

Point DelaunayTriangles::getRightMost() const {
    return rightMost;
}

Point DelaunayTriangles::getLeftMost() const {
    return leftMost;
}

void DelaunayTriangles::insert(const Point &a, const Point &b) {
    double x = b.x - a.x;
    double y = b.y - a.y;
    double theta = atan2(y, x) + M_PI;

    // Insert b into a
    auto list = this->adjancyList.at(a);
    std::list<Point>::iterator i;
    for (i = list->begin(); i != list->end(); ++i) {
        double theta2 = atan2(i->x - a.x, i->y - a.y) +  + M_PI;
        if(theta < theta2) {
            list->insert(i, b);
            break;
        }
    }
    if(i == list->end())
        list->insert(i, b);


    // Insert a into b
    theta = atan2(-y, -x) + M_PI;
    list = this->adjancyList.at(b);
    for (i = list->begin(); i != list->end(); ++i) {
        double theta2 = atan2(i->x - b.x, i->y - b.y) +  + M_PI;
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
    return det(4, mat) >= 0;
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
    Point z = vr.first(y);
    Point z_ = vl.first(x);
    Point z__ = vl.pred(x, z_);

    while(1) {
        if(isRightOf(z, Segment{x, y})) {
            z = vr.succ(z, y);
            y = z;
        } else {
            Segment l_ = {x, y};
            if(isRightOf(z__, l_)) {
                z__ = vl.pred(z__, x);
                x = z__;
            } else {
                return Segment{x, y};
            }
        }
    }
}

Segment hullUpper(const DelaunayTriangles &vl, const DelaunayTriangles &vr) {
    Point x = vl.getRightMost();
    Point y = vr.getLeftMost();
    Point z = vr.pred(y, vr.first(y));
    Point z__ = vl.first(x);

    while(1) {
        if(isLeftOf(z, Segment{x, y})) {
            z = vr.pred(z, y);
            y = z;
        } else {
            Segment l_ = {x, y};
            if(isLeftOf(z__, l_)) {
                z__ = vl.succ(z__, x);
                x = z__;
            } else {
                return Segment{x, y};
            }
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
