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

#include <ANN/ANN.h>

ANNpointArray read_points(const char *filename, int &count, int &dim)
{
    int res; 
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "error: could not open file: %s", filename);
        exit(1); 
    }

    res = fscanf(f, "%d %d\n", &count, &dim);
    if (res != 2) {
        fprintf(stderr, "error: invalid header: %s\n", filename);
        exit(1); 
    }

    if (count < 0) {
        fprintf(stderr, "error: invalid point count: %s: %d\n", filename, count);
        exit(1);
    }

    if (dim < 2) {
        fprintf(stderr, "error: invalid dimension: %s: %d\n", filename, dim);
        exit(1);
    }

    ANNpointArray pts = annAllocPts(count, dim); 
    char buffer[256];
    for (int i = 0; i < count; ++i) { 
        double value;

        if (fgets(buffer, 255, f) == 0) {
            fprintf(stderr, "error: short file: %s\n", filename);
            exit(1); 
        }

        char *start = buffer;
        char *end = start;
        for (int d = 0; d < dim; ++d) {
            while (*end != ',' && *end != ' ' && *end != 0) ++end;
            *end = 0;
            value = atof(start); 
            start = end + 1; 
            pts[i][d] = value;
        }
    }

    fclose(f);

    return pts; 
}

int main(int argc, char **argv)
{ 
    if (argc < 2) {
        std::cout << "usage: knn <pts> [queries] [nn] [epsilon]" << std::endl;
        exit(1);
    }

    int pt_count, dim; 
    ANNpointArray pts = read_points(argv[1], pt_count, dim);
   
    ANNkd_tree kt(pts, pt_count, dim);
    if (argc < 3) return 1;

    //read queries
    int q_count;
    int q_dim;

    ANNpointArray queries = read_points(argv[2], q_count, q_dim);

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
    ANNidx *nn_idx = new ANNidx[nn];
    ANNdist *dists = new ANNdist[nn];

    for (int i = 0; i < q_count; ++i) { 

        kt.annkSearch(queries[i], nn, nn_idx, dists, epsilon);

        std::cout << "query " << i << ": (";
        for (int d = 0; d < dim; ++d) { 
            std::cout << queries[i][d];
            if (d + 1 < dim) std::cout << ", ";
        }
        std::cout << ")\n";

        for (int j = 0; j < nn; ++j) { 
            std::cout << "("; 
            for (int d = 0; d < dim; ++d) {
                std::cout << pts[nn_idx[j]][d];
                if (d + 1 < dim) std::cout << ", ";
            }
            std::cout << ") " << dists[j] << "\n"; 
        } 
    }

    std::cout << "done." << std::endl;

    delete[] nn_idx;
    delete[] dists;
    annClose();

    return 0;
}
