#ifndef _CSV_H_
#define _CSV_H_

#include <vector>
#include <string>
#include "types.h"

using namespace std;

/* Returns a the points of the csv file */
vector<Point> readCSV(const string &filename);

#endif