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

#include "priority_queue.h"
struct PriorityQueue *build_priority_queue(size_t size)
{
    struct PriorityQueue *pq = malloc(sizeof(struct PriorityQueue));
  
    pq->entries = malloc(size * sizeof(struct PriorityQueueEntry));
    pq->length = 0; 
    pq->size = size;

    return pq;
}

void heapify(struct PriorityQueue *queue, size_t i)
{

    size_t l = i << 1;
    size_t r = l + 1;
    size_t smallest = i;

    if (l <= queue->length && queue->entries[l].priority < queue->entries[i].priority) {
        smallest = l;
    } 

    if (r <= queue->length && queue->entries[r].priority < queue->entries[smallest].priority) {
        smallest = r;
    }

    if (smallest != i) {
        struct PriorityQueueEntry t = queue->entries[i];
        queue->entries[i] = queue->entries[smallest];
        queue->entries[smallest] = t; 

        heapify(queue, smallest);
    } 
}

void priority_queue_push(struct PriorityQueue *queue, double priority, void *data)
{ 
    //adjust heap length
    ++queue->length;

    //resize if necessary
    if (queue->length == queue->size) {

        size_t new_size = queue->size << 1;
        struct PriorityQueueEntry *new = malloc(new_size * sizeof(struct PriorityQueueEntry));
        for (int i = 1; i < queue->length; ++i) {
            new[i] = queue->entries[i];
        }

        free(queue->entries);
        queue->entries = new;
        queue->size = new_size; 
    }

    //add to end of heap
    queue->entries[queue->length].priority = priority;
    queue->entries[queue->length].data = data;
  
    //place new entry in proper position in heap 
    size_t i = queue->length; 
    size_t parent = i >> 1;
    while (i != 1 && queue->entries[i].priority < queue->entries[parent].priority) { 
        struct PriorityQueueEntry t = queue->entries[i];
        queue->entries[i] = queue->entries[parent];
        queue->entries[parent] = t; 

        i = parent;
        parent = i >> 1;
    }
}

struct PriorityQueueEntry priority_queue_pop(struct PriorityQueue *queue)
{ 
    struct PriorityQueueEntry min = queue->entries[1]; 
    queue->entries[1] = queue->entries[queue->length]; 
    --queue->length;
    heapify(queue, 1); 

    return min; 
}

int priority_queue_length(struct PriorityQueue *queue)
{
    return queue->length;
}

void priority_queue_free(struct PriorityQueue *queue)
{
    free(queue->entries);
    free(queue);
}
