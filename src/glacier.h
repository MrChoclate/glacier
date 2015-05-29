#ifndef _GLACIER_H_
#define _GLACIER_H_

#include <vector>
#include <string>
#include <map>
#include "types.h"
#include "delaunay.h"

using namespace std;

vector<Triangle> make_triangles(const string &csv_filename);
Plan make_plan(const Triangle &t);
map<Triangle, Plan> make_plans(const vector<Triangle> &triangles);
bool in_triangle(const del_point2d_t &p, const Triangle &t);
Triangle* get_triangle(const del_point2d_t &p, const vector<Triangle> triangles);
void update(del_point2d_t &p, const Plan &plan);

void build_aerial_(const string &csv_filename, const string &in, const string &out);

#endif
