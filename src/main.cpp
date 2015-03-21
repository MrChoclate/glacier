#include <iostream>
#include "image_png.h"


int main(int argc, char const *argv[]) {

    ImagePNG img;
    img.load("img/2009072712_cam01.png");
    img.save("img/01.png");

    return 0;
}