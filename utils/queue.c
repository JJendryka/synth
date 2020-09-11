#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue* new_queue(int size)
{
    struct queue *pt = NULL;
    pt = (struct queue*)malloc(sizeof(struct queue));

    pt->items = (void**)malloc(size * sizeof(void*));
    pt->maxsize = size;
    pt->front = 0;
    pt->rear = -1;
    pt->size = 0;

    return pt;
}

int size(struct queue *pt)
{
    return pt->size;
}

int is_empty(struct queue *pt)
{
    return !size(pt);
}

void* front(struct queue *pt)
{
    if (is_empty(pt))
    {
        return NULL;
    }
    return pt->items[pt->front];
}

void* rear(struct queue *pt) {
    return pt->items[(pt->rear + 1) % pt->maxsize];
}

int enqueue(struct queue *pt)
{
    if (size(pt) == pt->maxsize)
    {
        return -1;
    }
    pt->rear = (pt->rear + 1) % pt->maxsize;
    pt->size++;

    return 0;
}

int dequeue(struct queue *pt)
{
    if (is_empty(pt))
    {
        return -1;
    }
    pt->front = (pt->front + 1) % pt->maxsize;
    pt->size--;
    return 0;
}