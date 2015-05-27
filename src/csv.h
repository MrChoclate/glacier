#ifndef _CSV_H_
#define _CSV_H_

#include <vector>
#include <string>
#include "delaunay.h"

using namespace std;

/* Returns a the points of the csv file */
vector<del_point2d_t> readCSV(const string &filename);

#endif