/*
Copyright (c) 2010 Daniel Minor 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "kdtree.h"

struct Point {
    static int dim;
    double *coords;

    Point()
    {
        coords = new double[dim];
    };

    virtual ~Point()
    {
        delete[] coords;
    } 

    Point(const Point &other)
    {
        Point();

        for (int d = 0; d < dim; ++d) {
            coords[d] = other.coords[d];
        }
    }

    double operator[](size_t idx) const {return coords[idx];}
    double &operator[](size_t idx) {return coords[idx];}
};

int Point::dim = 0;

Point *read_points(const char *filename, int &count, int &dim)
{ 
    std::ifstream ptf(filename);

    if (!ptf) {
        std::cout << "error: could not open file: " << filename << std::endl;
        exit(1); 
    }

    ptf >> count;
    ptf >> dim;

    if (count < 0) {
        std::cout << "error: invalid point count: " << count << std::endl;
        exit(1);
    }

    if (dim < 2) {
        std::cout << "error: invalid dimension: " << dim << std::endl;
        exit(1);
    }

    Point::dim = dim;

    Point *pts = new Point[count]; 
    for (int i = 0; i < count; ++i) { 
        char c;
        double value;

        for (int d = 0; d < dim; ++d) {
            ptf >> value; pts[i][d] = value;
            ptf >> c; 
        }
    }

    ptf.close();

    return pts; 
}


int main(int argc, char **argv)
{ 
    if (argc < 2) {
        std::cout << "usage: knn <pts> [queries] [nn] [epsilon]" << std::endl;
        exit(1);
    }

    int pt_count, dim;
    Point *pts = read_points(argv[1], pt_count, dim); 
   
    KdTree<Point, double> kt(dim, pts, pt_count);

    if (argc < 3) {
        return 1;
    }

    int q_count, q_dim;
    Point *queries = read_points(argv[2], q_count, q_dim); 
    if (dim != q_dim) {
        std::cerr << "error: query dim: " << q_dim;
        std::cerr << " does not match point dim: " << dim << std::endl;
        exit(1);
    }

    //how many nearest neighbours to retrieve
    int nn = 5;
    if (argc >= 4) nn = atoi(argv[3]);

    //read query epsilon
    double epsilon = 0.0;
    if (argc == 5) epsilon = atof(argv[4]);

    //run queries
    for (int i = 0; i < q_count; ++i) { 

        std::list<std::pair<Point *, double> > qr = kt.knn(nn, queries[i], epsilon);  

        std::cout << "query " << i << ": (";
        for (int d = 0; d < dim; ++d) { 
            std::cout << queries[i][d];
            if (d + 1 < dim) std::cout << ", ";
        }
        std::cout << ")\n";

        for (std::list<std::pair<Point *, double> >::iterator itor = qr.begin(); itor != qr.end(); ++itor) {
            std::cout << "("; 
            for (int d = 0; d < dim; ++d) {
                std::cout << (*itor->first)[d];
                if (d + 1 < dim) std::cout << ", ";
            }
            std::cout << ") " << itor->second << "\n"; 
        } 
    }

    std::cout << "done." << std::endl;

    delete[] pts;
    delete[] queries; 

    return 0;
}
