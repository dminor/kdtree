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

    ANNpointArray pts = annAllocPts(count, dim); 
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
        std::cout << "usage: knn <pts> [queries] [epsilon]" << std::endl;
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

    //read query epsilon
    double epsilon = 0.0;
    if (argc == 4) epsilon = atof(argv[3]);

    //run queries
    ANNidx *nn_idx = new ANNidx[5];
    ANNdist *dists = new ANNdist[5];

    for (int i = 0; i < q_count; ++i) { 

        kt.annkSearch(queries[i], 5, nn_idx, dists, epsilon);

        std::cout << "query " << i << ": (";
        for (int d = 0; d < dim; ++d) { 
            std::cout << queries[i][d];
            if (d + 1 < dim) std::cout << ", ";
        }
        std::cout << ")\n";

        for (int j = 0; j < 5; ++j) { 
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
