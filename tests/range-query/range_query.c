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

int main(int argc, char **argv)
{

    if (argc != 2) {
        printf("usage: range_query <input>\n");
        exit(1);
    }

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

    struct Point *pts = (struct Point *)malloc(pt_count * sizeof(struct Point));

    double x, y;
    for (int i = 0; i < pt_count; ++i) {
        fscanf(f, "%lf, %lf", &x, &y);
        pts[i].coord[0] = x;
        pts[i].coord[1] = y;
    }

    fclose(f);

    struct Kdnode *kt = build_kdtree(pts, pt_count, 0, 2); 

    return 0;
}
