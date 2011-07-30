/*
Copyright (c) 2011 Daniel Minor 

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

#include "priority_queue.h"

const int QUEUE_SIZE = 5;
const int TEST_SIZE = 10;

int main(int argc, char **argv)
{
    PriorityQueue<int> pq(QUEUE_SIZE);

    for (int i = 0; i < TEST_SIZE; ++i) {
        double priority = (double)rand()/(double)RAND_MAX;
        printf("inserting: %f %i\n", priority, i);
        pq.push(priority, i);
    }

    while(pq.length) {
        PriorityQueue<int>::Entry e = pq.pop();
        printf("%f %d\n", e.priority, e.data);
    }

    return 0;
}
