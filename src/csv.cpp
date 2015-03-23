#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "csv.h"
#include "types.h"

using namespace std;

vector<Point> readCSV(const string &filename) {
    vector<Point> res;
    int id;
    double x, y, x_utm, y_utm;
    ifstream file(filename);
    

    if(file.is_open()) {
        string line;
        getline(file, line); // Ignore the header
        while(getline(file, line)) {
            sscanf(line.c_str(), "GCP #%d\t%lf\t%lf\t%lf\t%lf", &id, &x, &y, &x_utm, &y_utm);
            Point p = {x, y, x_utm, y_utm};
            res.push_back(p);
        }
        file.close();
    }

    return res;
}
