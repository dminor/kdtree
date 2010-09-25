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

#include "kdtree.h"

typedef double Point[2];

static int point_in_range(Point *p, double *range, size_t dim)
{
    for (int i = 0; i < dim; ++i) {
        if (range[i*2] > (*p)[i] || range[i*2+1] < (*p)[i]) return 0;
    }

    return 1; 
} 

std::vector<Point *> linear_range_query(int pt_count, Point *pts, double *range)
{ 
    std::vector<Point *> qr;

    for (int i = 0; i < pt_count; ++i) {
        if (point_in_range(&pts[i], range, 2)) qr.push_back(&pts[i]);
    }

    return qr;
}

int main(int argc, char **argv)
{ 
    if (argc != 3) {
        printf("usage: range_query <pts> <queries>\n");
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

    double x, y;
    for (int i = 0; i < pt_count; ++i) {
        fscanf(f, "%lf, %lf", &x, &y);
        pts[i][0] = x;
        pts[i][1] = y;
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

    if (pt_count < 0) {
        printf("error: invalid query count %d\n", q_count);
        exit(1);
    }

    double *ranges = new double[4 * q_count];

    for (int i = 0; i < q_count; ++i) {
        fscanf(f, "%lf, %lf, %lf, %lf", &ranges[i*4], &ranges[i*4+1], &ranges[i*4+2], &ranges[i*4+3]);
    }

    fclose(f);

    KdTree<Point> kt(2, pts, pt_count);

    //run queries
    for (int i = 0; i < q_count; ++i) { 

        std::vector<Point *> kqr = kt.range_query(&ranges[i*4]);  
        std::vector<Point *> lqr = linear_range_query(pt_count, pts, &ranges[i*4]);  

        if (kqr.size() != lqr.size()) {
            printf("error: kdtree and linear do not agree for query %d\n", i + 1);
            printf("range: %.1f %.1f %.1f %.1f\n", ranges[i*4], ranges[i*4+1], ranges[i*4+2], ranges[i*4+3]);
            printf("(kdtree) found %d points...\n", kqr.size());
            for (size_t i = 0; i < kqr.size(); ++i) {
                printf("%d -> (%.1f, %.1f)\n", i + 1, *(kqr[i])[0], *(kqr[i])[1]);
            }
            printf("(linear) found %d points...\n", lqr.size());
            for (size_t i = 0; i < lqr.size(); ++i) {
                printf("%d -> (%.1f, %.1f)\n", i + 1, *(lqr[i])[0], *(lqr[i])[1]);
            }

            delete[] pts;
            delete[] ranges;

            return -1;
        } 
    }

    delete[] pts;
    delete[] ranges;

    return 0;
}
