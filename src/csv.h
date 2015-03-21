#ifndef _CSV_H_
#define _CSV_H_

#include <vector>
#include <string>

using namespace std;

struct Point {
    double x, y, x_utm, y_utm;
};

typedef struct Point Point;

/* Returns a the points of the csv file */
vector<Point> readCSV(const string &filename);

#endif