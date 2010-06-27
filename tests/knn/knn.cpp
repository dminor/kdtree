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

#include <stdio.h>
#include <stdlib.h>

#include "kdtree.h"

struct Point {
    double coords[2];

    Point() {};

    Point(const Point &other)
    {
        coords[0] = other.coords[0];
        coords[1] = other.coords[1]; 
    }

    double operator[](size_t idx) const {return coords[idx];}
    double &operator[](size_t idx) {return coords[idx];}
};

double query_x;
double query_y;

bool pt_lt(const Point pt1, const Point pt2)
{ 
    double d1 = (pt1[0]-query_x)*(pt1[0]-query_x)+(pt1[1]-query_y)*(pt1[1]-query_y);
    double d2 = (pt2[0]-query_x)*(pt2[0]-query_x)+(pt2[1]-query_y)*(pt2[1]-query_y);

    return d1 < d2;
}

std::vector<Point *> sort_knn(Point *pts, size_t pt_count, size_t k, Point pt)
{
    std::vector<Point *> qr;

    query_x = pt[0]; query_y = pt[1];
    std::sort(pts, pts + pt_count, pt_lt);

    for (int i = 0; i < k; ++i) {
        qr.push_back(&pts[i]);
    } 

    return qr;
}

int main(int argc, char **argv)
{ 
    if (argc != 3) {
        printf("usage: knn <pts> <queries>\n");
        exit(1);
    }

    //read points
    int pt_count;

    FILE *f = fopen(argv[1], "r");

    if (!f) {
        printf("error: could not open points file: %s\n", argv[1]);
        exit(1); 
    }

    fscanf(f, "%d", &pt_count);

    if (pt_count < 0) {
        printf("error: invalid point count %d\n", pt_count);
        exit(1);
    }

    Point *pts = new Point[pt_count]; 
    for (int i = 0; i < pt_count; ++i) {
        fscanf(f, "%lf, %lf", &(pts[i][0]), &(pts[i][1]));
    }

    fclose(f);

    //read queries
    int q_count;

    f = fopen(argv[2], "r");

    if (!f) {
        printf("error: could not open query file: %s\n", argv[2]);
        exit(1); 
    }

    fscanf(f, "%d", &q_count);

    if (q_count < 0) {
        printf("error: invalid query count %d\n", q_count);
        exit(1);
    }

    Point *queries = new Point[q_count]; 

    for (int i = 0; i < q_count; ++i) {
        fscanf(f, "%lf, %lf", &(queries[i][0]), &(queries[i][1]));
    }

    fclose(f);

    KdTree<Point> kt(2, pts, pt_count);

    //run queries
    for (int i = 0; i < q_count; ++i) { 

        std::vector<Point *> kqr = kt.knn(5, queries[i]);  
        std::vector<Point *> lqr = sort_knn(pts, pt_count, 5, queries[i]);  

        for (int j = 0; j < 5; ++j) {
            double x = (*lqr[j])[0] - (*kqr[j])[0]; 
            double y = (*lqr[j])[1] - (*kqr[j])[1];

            if ((x*x + y*y) > 0.0001) {
                printf("error: kdtree nearest neighbour does not match sort nearest neighbour\n");

                printf("query: %d %.3f %.3f\n", i, queries[i][0], queries[i][1]);
                double d1 = (queries[i][0]-(*kqr[j])[0])*(queries[i][0]-(*kqr[j])[0]) + (queries[i][1]-(*kqr[j])[1])*(queries[i][1]-(*kqr[j])[1]);
                double d2 = (queries[i][0]-(*lqr[j])[0])*(queries[i][0]-(*lqr[j])[0]) + (queries[i][1]-(*lqr[j])[1])*(queries[i][1]-(*lqr[j])[1]);
                printf("%d kqr: %.3f %.3f %.3f ", j, (*kqr[j])[0], (*kqr[j])[1], d1);
                printf("lqr: %.3f %.3f %.3f\n", (*lqr[j])[0], (*lqr[j])[1], d2);

                return 1;
            }

        } 
    }

    delete[] pts;
    delete[] queries;

    return 0;
}
