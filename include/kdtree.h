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

#ifndef KD_TREE_H_
#define KD_TREE_H_

#include <stdlib.h>

struct Point {
    double coord[2];
    void *v;
};

struct Kdnode {
    struct Kdnode *left, *right;
    struct Point pt;
    size_t children;
};

struct KdtreeRangeQueryResult {
    size_t count;
    struct Point *pts;
};

struct KdtreeKNNResult {
    size_t count;
    struct Point *pts;
    double *distances;
};

struct Kdnode *build_kdtree(struct Point *pts, size_t pt_count, size_t depth, size_t dim);
struct KdtreeRangeQueryResult kdtree_range_query(struct Kdnode *tree, double *range, size_t dim); 
struct KdtreeKNNResult kdtree_knn(struct Kdnode *tree, size_t k, double *pt, size_t dim);

#endif

