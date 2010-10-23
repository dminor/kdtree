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

#include <cmath>
#include <cstdlib>

#include <limits>
#include <vector>

template<class Point> class KdTree {

public:

    template<class T> struct Node {
        Node *left, *right;
        T pt;
        size_t children;
    };

    KdTree(size_t dim, Point *pts, size_t n) : dim(dim)
    {
        root = build_kdtree(pts, n, 0);
    }

    virtual ~KdTree()
    {
        std::vector<Node<Point> *> nodes;
        nodes.push_back(root);
        while (!nodes.empty()) {
            Node<Point> *n = nodes.back();
            nodes.pop_back();

            if (n->left) nodes.push_back(n->left);
            if (n->right) nodes.push_back(n->right);
            delete n;
        }
    }

    std::vector<Point *> range_query(double *range)
    {
        //set up region
        double *region = new double[2 * dim]; 
        for (int i = 0; i < dim; ++i) {
            region[i] = -std::numeric_limits<double>::max();
            region[i + 1] = std::numeric_limits<double>::min(); 
        }

        //run query
        std::vector<Point *> qr = range_query(root, range, region, 0);

        //clean up and return result;
        delete[] region;

        return qr;

    }

    std::vector<std::pair<Point *, double> > knn(size_t k, const Point &pt, double eps) 
    {
        std::vector<std::pair<Point *, double> > qr; 
        qr.reserve(k);
        for (size_t i = 0; i < k; ++i) {
            qr[i].first = 0;
            qr[i].second = std::numeric_limits<double>::max();
        }
 
        knn_search(qr, root, k, pt, eps, 0);
        return qr;
    }

    Node<Point> *root;

private:

    size_t dim;

    Node<Point> *build_kdtree(Point *pts, size_t pt_count, size_t depth)
    {
        Node<Point> *result = new Node<Point>; 

        if (pt_count == 1) {
            //leaf node, store point and return
            result->children = 1;
            result->left = result->right = 0;
            for(size_t i = 0; i < dim; ++i) result->pt[i] = pts[0][i];
        } else {

            //branch coordinate
            size_t coord = depth % dim; 

            //find median (has side effect of partitioning input array around median)
            size_t median_index = (pt_count / 2) >> 1 << 1;
            double median = select_order(median_index, pts, pt_count, coord);

            //recursively build tree
            result->left = build_kdtree(pts, median_index + 1, depth + 1); 
            result->right = build_kdtree(&pts[median_index + 1], pt_count - median_index - 1, depth + 1);

            //keep track of number of children
            result->children = result->left->children + result->right->children;

            //store median value
            for (int i = 0; i < dim; ++i) result->pt[i] = 0;
            result->pt[(coord + 1) % dim] = median;
        } 

        return result;
    }

    void swap(Point &a, Point &b)
    {
        double t;

        for (size_t i = 0; i < dim; ++i) {
            t = a[i];
            a[i] = b[i];
            b[i] = t; 
        }
    }

    size_t partition(size_t start, size_t end, Point *pts, size_t coord)
    { 
        //choose pivot and place at end
        size_t pivot = start + rand() % (end - start); 
        swap(pts[pivot], pts[end]);

        //get pivot value
        double value = pts[end][coord];

        //move values around pivot
        size_t i = start;
        for (size_t j = start; j < end; ++j) { 
            if (pt_lt(coord, pts[j], pts[end])) {
                swap(pts[i], pts[j]);
                ++i;
            }
        }
        
        swap(pts[i], pts[end]);

        return i; 
    } 

    double select_order(size_t i, Point *pts, size_t pt_count, size_t coord)
    {
        size_t start = 0;
        size_t end = pt_count - 1; 

        while (1) {

            if (start == end) return pts[start][coord];
     
            size_t pivot = partition(start, end, pts, coord);

            if (i == pivot) {
                return pts[pivot][coord];
            } else if (i < pivot) {
                end = pivot - 1;
            } else {
                start = pivot + 1;
            } 
        } 
    } 

    int pt_lt(size_t coord, const Point &a, const Point &b) const
    {
        //if points are unequal, do direct comparison
        if (a[coord] != b[coord]) {
            return a[coord] < b[coord];
        } else {
            //otherwise, compare in lexicographic order
            size_t i = (coord + 1) % dim; 
            while (a[i] == b[i] && i != coord) i = (i + 1) % dim; 
            return a[i] <= b[i];
        }
    } 

    int point_in_range(Point *p, double *range)
    {
        for (int i = 0; i < dim; ++i) {
            if (range[i*2] > (*p)[i] || range[i*2+1] < (*p)[i]) return 0;
        }

        return 1; 
    }

    int range_contains_region(double *range, double *region)
    { 
        for (int i = 0; i < dim; ++i) {
            if (range[i*2] > region[i*2] || range[i*2+1] < region[i*2+1]) return 0;
            if (range[i*2] > region[i*2+1] || range[i*2+1] < region[i*2]) return 0;
        }

        return 1; 
    }

    int region_intersects_range(double *range, double *region)
    {
        int intersects = 0;
        for (int i = 0; i < dim; ++i) {
            if (range[i*2] < region[i*2] || range[i*2+1] > region[i*2+1]) intersects = 1; 
            if (range[i*2] < region[i*2+1] || range[i*2+1] > region[i*2]) intersects = 1; 
        }

        return intersects; 
    }

    void report_subtree(Node<Point> *tree, std::vector<Point *> &qr)
    { 
        if (tree->left == 0 && tree->right == 0) {
            qr.push_back(&tree->pt);
        } else {
            //recurse through tree
            report_subtree(tree->left, qr);
            report_subtree(tree->right, qr);
        }
    }

    std::vector<Point *> range_query(Node<Point> *tree, double *range, double *region, size_t depth)
    {
        std::vector<Point *> qr;

        //leaf node
        if (tree->left == 0 && tree->right == 0) { 
            if (point_in_range(&tree->pt, range)) {
                qr.push_back(&tree->pt);
            }
        } else {

            std::vector<Point *> lqr, rqr;

            double split_value = tree->pt[(depth + 1) % dim];

            //left subtree -- update region
            int changed_index = 2 * (depth % dim) + 1;

            double changed_value = region[changed_index];    
            region[changed_index] = split_value; 

            if (range_contains_region(range, region)) {
                if (tree->left->children) {
                    report_subtree(tree->left, lqr);
                }
            } else if (region_intersects_range(region, range)) {
                lqr = range_query(tree->left, range, region, depth+1); 
            }

            //restore region 
            region[changed_index] = changed_value; 

            //right subtree -- update region 
            changed_index = 2 * (depth % dim);
            changed_value = region[changed_index];    
            region[changed_index] = split_value; 

            if (range_contains_region(range, region)) {
                if (tree->right->children) {
                    report_subtree(tree->right, rqr);
                }
            } else if (region_intersects_range(region, range)) {
                rqr = range_query(tree->right, range, region, depth+1); 
            }

            //restore region 
            region[changed_index] = changed_value; 

            //collect results
            qr.insert(qr.end(), lqr.begin(), lqr.end());
            qr.insert(qr.end(), rqr.begin(), rqr.end()); 
        }

        return qr; 
    }

    void knn_search(std::vector<std::pair<Point *, double> > &qr, Node<Point> *node, size_t k, const Point &pt, double eps, size_t depth)
    { 
        if (node->left == 0 && node->right == 0) {

            //calculate distance from query point to this point
            double d = 0.0; 
            for (int i = 0; i < dim; ++i) {
                d += (node->pt[i]-pt[i]) * (node->pt[i]-pt[i]); 
            }
            d = sqrt(d);

            //insert point in proper order, for small k this will be fast enough
            for (size_t i = 0; i < k; ++i) {
                if (qr[i].second > d) {
                    for (size_t j = k - 1; j > i; --j) {
                        qr[j] = qr[j - 1];
                    }
                    qr[i].first = &node->pt;
                    qr[i].second = d;
                    break; 
                }
            } 
        } else { 
            if (pt[depth % dim] < node->pt[(depth + 1) % dim]) { 
                knn_search(qr, node->left, k, pt, eps, depth + 1);

                //if other side closer than farthest point, search it as well
                double d = (1 + eps)*qr[k - 1].second; 
                if (abs(node->pt[(depth + 1) % dim] - pt[depth % dim]) < d) { 
                    knn_search(qr, node->right, k, pt, eps, depth + 1);
                }

            } else {
                knn_search(qr, node->right, k, pt, eps, depth + 1);

                //if other side closer than farthest point, search it as well
                double d = (1 + eps)*qr[k - 1].second; 
                if (abs(node->pt[(depth + 1) % dim] - pt[depth % dim]) < d) {
                    knn_search(qr, node->left, k, pt, eps, depth + 1);
                }
            }
        }
    }

};

#endif

