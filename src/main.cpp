#include <iostream>
#include <algorithm>
#include "image_png.h"
#include "csv.h"
#include "types.h"
#include "delaunay.h"

int main(int argc, char const *argv[]) {

    std::vector<del_point2d_t> csv = readCSV("csv/Points-Controle_cam1.csv");
    

    delaunay2d_t* triangles = delaunay2d_from(csv.data(), csv.size());

    std::cout << triangles->num_faces << std::endl;

    unsigned int* p = triangles->faces;
    for(unsigned int i = 0; i < triangles->num_faces; i++, p++) {
        unsigned int num_verts =  *p;
        std::cout << num_verts << " ";
        for (unsigned int j = 0; j < num_verts; j++, p++) {   
            std::cout << *p << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}