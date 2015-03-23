#include <iostream>
#include "image_png.h"
#include "csv.h"
#include "types.h"

using namespace std;

int main(int argc, char const *argv[]) {

    vector<Point> csv = readCSV("csv/Points-Controle_cam1.csv");
    cout << csv.size();

    for (unsigned int i = 0; i < csv.size(); ++i)
    {
        cout << csv[i].x << " " << csv[i].y << " " << csv[i].x_utm << " " << csv[i].y_utm << endl;
    }

    return 0;
}