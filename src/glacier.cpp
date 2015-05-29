#include <vector>
#include <map>
#include <string>
#include <Eigen/Dense>
#include <iostream>
#include "types.h"
#include "delaunay.h"
#include "csv.h"
#include "image_png.h"

using namespace std;
using namespace Eigen;

vector<Triangle> make_triangles(const string &csv_filename) {

    vector<del_point2d_t> csv = readCSV(csv_filename);
    
    delaunay2d_t* triangles = delaunay2d_from(csv.data(), csv.size());

    unsigned int* p = triangles->faces;
    vector<Triangle> ts;
    for(unsigned int i = 0; i < triangles->num_faces; i++) {
        unsigned int num_verts =  *p;
        Triangle t;
        p++;
        for (unsigned int j = 0; j < num_verts; j++, p++) {   
            if(num_verts == 3) {
                switch(j) {
                    case 0:
                        t.a = csv[*p];
                        break;
                    case 1:
                        t.b = csv[*p];
                        break;
                    case 2:
                        t.c = csv[*p];
                        break;
                }
            }
        }
        if(num_verts == 3)
            ts.push_back(t);
    }
    return ts;
}

Plan make_plan(const Triangle &t) {
    Plan p;
    Matrix3f A;
    Vector3f b, b_;

    A << t.a.x, t.a.y, 1,
         t.b.x, t.b.y, 1,
         t.c.x, t.c.y, 1;
    b << t.a.x_utm, t.b.x_utm, t.c.x_utm;
    b_ << t.a.y_utm, t.b.y_utm, t.c.y_utm;

    Vector3f x = A.fullPivLu().solve(b);
    p.a = x(0);
    p.b = x(1);
    p.c = x(2);

    Vector3f x_ = A.fullPivLu().solve(b_);
    p.a_ = x_(0);
    p.b_ = x_(1);
    p.c_ = x_(2);

    return p;
}

vector<Plan> make_plans(const vector<Triangle> &triangles) {
    vector<Plan> plans;

    for (auto i = triangles.begin(); i != triangles.end(); ++i) {
        plans.push_back(make_plan(*i));
    }

    return plans;
}

bool isRightOf(const del_point2d_t &z, const del_point2d_t &a, const del_point2d_t &b) {
    double x1 = z.x - a.x;
    double x2 = b.x - a.x;

    double y1 = z.y - a.y;
    double y2 = b.y - a.y;

    double determinant = x1 * y2 - x2 * y1;

    return determinant > 0;
}

bool in_triangle(const del_point2d_t &p, const Triangle &t) {
    bool cond1 = isRightOf(p, t.a, t.b) && isRightOf(p, t.b, t.c) && isRightOf(p, t.c, t.a);
    bool cond2 = !isRightOf(p, t.a, t.b) && !isRightOf(p, t.b, t.c) && !isRightOf(p, t.c, t.a);
    return cond1 || cond2;
}

unsigned int get_triangle(const del_point2d_t &p, vector<Triangle> triangles) {
    for (unsigned int i = 0; i < triangles.size(); i++) {
        if(in_triangle(p, triangles[i])) {
            return i;
        }
    }
    return -1;
}

void update(del_point2d_t &p, const Plan &plan) {
    p.x_utm = plan.a * p.x + plan.b * p.y + plan.c;
    p.y_utm = plan.a_ * p.x + plan.b_ * p.y + plan.c_;
}

vector<del_point2d_t> build_points(const Triangle &t) {
    Plan plan = make_plan(t);
    int x_min = t.a.x; 
    x_min = (x_min > t.b.x) ? t.b.x : x_min;
    x_min = (x_min > t.c.x) ? t.c.x : x_min;

    int y_min = t.a.y; 
    y_min = (y_min > t.b.y) ? t.b.y : y_min;
    y_min = (y_min > t.c.y) ? t.c.y : y_min;

    int x_max = t.a.x; 
    x_max = (x_max < t.b.x) ? t.b.x : x_max;
    x_max = (x_max < t.c.x) ? t.c.x : x_max;

    int y_max = t.a.y; 
    y_max = (y_max < t.b.y) ? t.b.y : y_max;
    y_max = (y_max < t.c.y) ? t.c.y : y_max;

    vector<del_point2d_t> res;
    for(int i = x_min; i <= x_max; i++) {
        for(int j = y_min; j <= y_max; j++) {
            del_point2d_t p = del_point2d_t{(double) i, (double) j};
            if(in_triangle(p, t)) {
                update(p, plan);
                res.push_back(p);
            }
        }
    }
    return res;
}

vector<del_point2d_t> build_points(vector<Triangle> triangles) {
    vector<del_point2d_t> projection;
    for (auto i = triangles.begin(); i != triangles.end(); ++i) {
        auto res = build_points(*i);
        projection.reserve(projection.size() + res.size());
        projection.insert(projection.end(), res.begin(), res.end());
    }
    return projection;
}

void get_min_max(float &x_min, float &x_max, float &y_min, float &y_max, const vector<del_point2d_t> &points) {
    x_min = points[0].x_utm;
    x_max = x_min;
    y_min = points[0].y_utm;
    y_max = y_min;

    for (auto i = points.begin(); i != points.end(); ++i) {
        if(x_min > i->x_utm)
            x_min = i->x_utm;
        if(x_max < i->x_utm)
            x_max = i->x_utm;

        if(y_min > i->y_utm)
            y_min = i->y_utm;
        if(y_max < i->y_utm)
            y_max = i->y_utm;
    }
}

bool is_blank(const ImagePNG &img, const unsigned int x, const unsigned int y) {
    return img.red(y, x) == 255 && img.green(y, x) == 255 && img.blue(y, x) == 255;
}

ImagePNG smooth(const ImagePNG &img) {
    ImagePNG res = img;
    for(unsigned int x = 1; x < img.getWidth() - 1; x++) {
        for(unsigned int y = 1; y < img.getHeight() - 1; y++) {
            if(is_blank(img, x, y)) {
                int count = 0;
                int r = 0, g = 0, b = 0;
                for(unsigned int i = -1; i <= 1; i++) {
                    for (unsigned int j = -1; j <= 1; j++) {
                        if(i != j && !is_blank(img, x + i, y + j)) {
                            count++;
                            r += img.red(y + j, x + i);
                            g += img.green(y + j, x + i);
                            b += img.blue(y + j, x + i);
                        }
                    }
                }
                if(count != 0) {
                    res.red(y, x) = r / count;
                    res.green(y, x) = g / count;
                    res.blue(y, x) = b / count;
                }
            }
        }
    }
    return res;
}

void build_aerial_(const string &csv_filename, const string &in, const string &out) {
    ImagePNG file_in = ImagePNG();
    ImagePNG file_out = ImagePNG();

    file_in.load(in);

    auto triangles = make_triangles(csv_filename);
    auto projection = build_points(triangles);

    float x_max, x_min, y_max, y_min;
    get_min_max(x_min, x_max, y_min, y_max, projection);
    
    file_out.newImage(ceil(y_max - y_min) + 2, ceil(x_max - x_min) + 2);
    for (auto i = projection.begin(); i != projection.end(); ++i) {
        file_out.red(ceil(i->y_utm - y_min), ceil(i->x_utm - x_min)) = file_in.red(i->y, i->x);
        file_out.green(ceil(i->y_utm - y_min), ceil(i->x_utm - x_min)) = file_in.green(i->y, i->x);
        file_out.blue(ceil(i->y_utm - y_min), ceil(i->x_utm - x_min)) = file_in.blue(i->y, i->x);
        file_out.alpha(ceil(i->y_utm - y_min), ceil(i->x_utm - x_min)) = file_in.alpha(i->y, i->x);
    }

    file_out = smooth(file_out);
    file_out.save(out);

    cout << "Creating: " << out << "; x goes from " << (int) floor(x_min) << " to " << (int) ceil(x_min + file_out.getWidth());
    cout << " and y goes from " << (int) floor(y_min) << " to " << (int) ceil(y_min + file_out.getHeight()) << endl;
}