#include <iostream>
#include <algorithm>
#include "image_png.h"
#include "csv.h"
#include "types.h"
#include "delaunay.h"

using namespace std;

int main(int argc, char const *argv[]) {

    vector<Point> csv = readCSV("csv/Points-Controle_cam1.csv");
    //vector<Point> csv = {Point{0, 0}, Point{1, 1}, Point{2, -1}, Point{3, 0}, Point{3, 1}};
    sort(csv.begin(), csv.end());

    DelaunayTriangles triangles = DelaunayTriangles(csv);

    return 0;
}