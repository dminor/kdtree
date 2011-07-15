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

#include "kdtree.h"

#include <float.h>
#include <cstdio>
#include <cstdlib>

typedef double Point[2];

void render_tree(FILE *f, struct KdTree<Point>::Node *tree, size_t depth, double x1, double x2, double y1, double y2)
{
    //check for empty branch
    if (!tree) return;

    if (tree->left == 0 && tree->right == 0) {
        //leaf
        fprintf(f, "%.0f %.0f draw-point\n", (*tree->pt)[0], (*tree->pt)[1]);
    } else { 
        //branch 
        if (depth < 1) fprintf(f, "4 setlinewidth\n");
        else if (depth < 2) fprintf(f, "3 setlinewidth\n"); 
        else if (depth < 4) fprintf(f, "2 setlinewidth\n");
        else fprintf(f, "1 setlinewidth\n");

        if (depth % 2 == 1) {
            fprintf(f, "%.0f %.0f %.0f h-line\n", x1, x2, tree->median);
            render_tree(f, tree->left, depth + 1, x1, x2, y1, tree->median);
            render_tree(f, tree->right, depth + 1, x1, x2, tree->median, y2);
        } else {
            fprintf(f, "%.0f %.0f %.0f v-line\n", tree->median, y1, y2);
            render_tree(f, tree->left, depth + 1, x1, tree->median, y1, y2);
            render_tree(f, tree->right, depth + 1, tree->median, x2, y1, y2);
        }

        fprintf(f, "%.0f %.0f draw-point\n", (*tree->pt)[0], (*tree->pt)[1]);
    }
}

int main(int argc, char **argv)
{ 
    if (argc != 3) {
        printf("usage: render_tree <input> <output>\n");
        exit(1);
    }

    int pt_count, dim;

    FILE *f = fopen(argv[1], "r");

    if (!f) {
        printf("error: could not open points file: %s\n", argv[1]);
        exit(1); 
    }

    fscanf(f, "%d %d", &pt_count, &dim);

    if (pt_count < 0) {
        printf("error: invalid point count %d\n", pt_count);
        exit(1);
    }

    if (dim != 2) {
        printf("error: can not render non 2D points\n");
        exit(1);
    }

    Point *pts = new Point[pt_count];

    double x, y;
    double x1 = DBL_MAX, x2 = DBL_MIN, y1 = DBL_MAX, y2 = DBL_MIN;
    for (int i = 0; i < pt_count; ++i) {
        fscanf(f, "%lf, %lf", &x, &y);
        pts[i][0] = x;
        pts[i][1] = y;

        //set up bounds
        if (x < x1) x1 = x;
        if (x > x2) x2 = x;
        if (y < y1) y1 = y;
        if (y > y2) y2 = y;
    }


    fclose(f);

    KdTree<Point> kt(2, pts, pt_count);

    f = fopen(argv[2], "w");
    if (!f) {
        printf("error: could not open output file: %s\n", argv[2]);
        exit(1); 
    }

    fprintf(f, "%%\n");

    //define point function for later
    fprintf(f, "/draw-point {\n");
    fprintf(f, "    /y exch def\n");
    fprintf(f, "    /x exch def\n");
    fprintf(f, "    gsave\n");
    fprintf(f, "    newpath\n");
    fprintf(f, "    0.5 0.5 0.7 setrgbcolor\n");
    fprintf(f, "    x y 2 0 360 arc\n");
    fprintf(f, "    closepath\n");
    fprintf(f, "    fill\n");
    fprintf(f, "    newpath\n");
    fprintf(f, "    0.4 setgray\n");
    fprintf(f, "    x y 2 0 360 arc\n");
    fprintf(f, "    closepath\n");
    fprintf(f, "    stroke\n");
    fprintf(f, "    grestore\n");
    fprintf(f, "} def\n");

    //vertical line
    fprintf(f, "/v-line {\n");
    fprintf(f, "    /y2 exch def\n");
    fprintf(f, "    /y1 exch def\n");
    fprintf(f, "    /x exch def\n");
    fprintf(f, "    gsave\n");
    fprintf(f, "    0.7 setgray\n");
    fprintf(f, "    newpath\n");
    fprintf(f, "    x y1 moveto\n");
    fprintf(f, "    x y2 lineto\n");
    fprintf(f, "    closepath\n");
    fprintf(f, "    stroke \n");
    fprintf(f, "    grestore\n");
    fprintf(f, "} def\n");

    //horizontal line
    fprintf(f, "/h-line {\n");
    fprintf(f, "    /y exch def\n");
    fprintf(f, "    /x2 exch def\n");
    fprintf(f, "    /x1 exch def\n");
    fprintf(f, "    gsave\n");
    fprintf(f, "    0.7 setgray\n");
    fprintf(f, "    newpath\n");
    fprintf(f, "    x1 y moveto\n");
    fprintf(f, "    x2 y lineto\n");
    fprintf(f, "    closepath\n");
    fprintf(f, "    stroke \n");
    fprintf(f, "    grestore\n");
    fprintf(f, "} def\n");

    fprintf(f, "60 240 translate\n");

    render_tree(f, kt.root, 0, x1, x2, y1, y2); 

    fclose(f);

}
