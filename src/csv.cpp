#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "csv.h"
#include "delaunay.h"

using namespace std;

vector<del_point2d_t> readCSV(const string &filename) {
    vector<del_point2d_t> res;
    int id;
    double x, y, x_utm, y_utm;
    ifstream file(filename);
    

    if(file.is_open()) {
        string line;
        getline(file, line); // Ignore the header
        while(getline(file, line)) {
            sscanf(line.c_str(), "GCP #%d\t%lf\t%lf\t%lf\t%lf", &id, &x, &y, &x_utm, &y_utm);
            del_point2d_t p = {x, y, x_utm, y_utm};
            res.push_back(p);
        }
        file.close();
    }

    return res;
}
