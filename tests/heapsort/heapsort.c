
#include <stdio.h>

#include "priority_queue.h"

const int N = 12;

int compare_double(void *a, void *b)
{
    return *(double *)a < *(double *)b ? -1 : 1; 
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: test-queue <size>\n>");
        return 1;
    }

    int N = atoi(argv[1]);
    if (N < 1) {
        printf("error: invalid test size\n");
    }

    double *numbers = malloc(N * sizeof(double));

    struct PriorityQueue *pq = build_priority_queue(N);

    for (int i = 0; i < N; ++i) {
        numbers[i] = (double)rand() / (double)RAND_MAX;
        priority_queue_push(pq, numbers[i], 0);
    }

    qsort(numbers, N, sizeof(double), compare_double);

    for (int i = 0; i < N; ++i) {
        struct PriorityQueueEntry e = priority_queue_pop(pq);

        if (e.priority != numbers[i]) { 
            printf("error: heap sort does not match library qsort\n");
            break;
        }
    }

    priority_queue_free(pq);
    free(numbers);

    return 0;
}
