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
#include <string.h>

#include "kdtree.h"

int pt_lt(size_t dim, size_t coord, struct Point a, struct Point b)
{
    //if points are unequal, do direct comparison
    if (a.coord[coord] != b.coord[coord]) {
        return a.coord[coord] < b.coord[coord];
    } else {
        //otherwise, compare in lexicographic order
        size_t i = (coord + 1) % dim; 
        while (a.coord[i] == b.coord[i] && i != coord) i = (i + 1) % dim; 
        return a.coord[i] <= b.coord[i];
    }
}

static size_t partition(size_t start, size_t end, struct Point *pts, size_t dim, size_t coord)
{
    struct Point t;

    //choose pivot and place at end
    int pivot = start + rand() % (end - start); 
    t = pts[pivot]; 
    pts[pivot] = pts[end];
    pts[end] = t;

    //get pivot value
    double value = pts[end].coord[coord];

    //move values around pivot
    size_t i = start;
    for (size_t j = start; j < end; ++j) { 
        if (pt_lt(dim, coord, pts[j], pts[end])) {
            t = pts[i];
            pts[i] = pts[j];
            pts[j] = t;
            i += 1;
        }
    }

    t = pts[i]; 
    pts[i] = pts[end];
    pts[end] = t;

    return i; 
} 

double select_order(size_t i, struct Point *pts, size_t pt_count, size_t dim, size_t coord)
{
    size_t start = 0;
    size_t end = pt_count - 1; 

    while (1) {

        if (start == end) return pts[start].coord[coord];
 
        size_t pivot = partition(start, end, pts, dim, coord);

        if (i == pivot) {
            return pts[pivot].coord[coord];
        } else if (i < pivot) {
            end = pivot - 1;
        } else {
            start = pivot + 1;
        } 
    } 
}

struct Kdnode *build_kdtree(struct Point *pts, size_t pt_count, size_t depth, size_t dim)
{
    struct Kdnode *result = (struct Kdnode *)malloc(sizeof(struct Kdnode));

    printf("%d %d\n", pt_count, depth);

    if (pt_count == 1) {
        //leaf node, store point and return
        result->left = result->right = 0;
        memcpy(&result->pt, pts, sizeof(struct Point));
    } else {

        //branch coordinate
        size_t coord = depth % dim; 

        //find median (has side effect of partitioning input array around median)
        size_t median_index = (pt_count / 2) >> 1 << 1;
        double median = select_order(median_index, pts, pt_count, dim, coord);

        printf("%.3f %d %d\n", median, median_index, pt_count - median_index);

        //recursively build tree
        result->left = build_kdtree(pts, median_index + 1, depth + 1, dim);
        result->left->parent = result;

        result->right = build_kdtree(&pts[median_index + 1], pt_count - median_index - 1, depth + 1, dim);
        result->right->parent = result;

        //store median value
        memset(&result->pt.coord, dim * sizeof(double), 0);
        result->pt.coord[(coord + 1) % dim] = median;
    } 

    return result;
}
