#ifndef SYNTH_QUEUE_H
#define SYNTH_QUEUE_H

struct queue {
    void **items;
    int maxsize;
    int front;
    int rear;
    int size;
};

struct queue* new_queue(int size);
int size(struct queue *pt);
int is_empty(struct queue *pt);
void* front(struct queue *pt);
void* rear(struct queue *pt);
int enqueue(struct queue *pt);
int dequeue(struct queue *pt);

#endif //SYNTH_QUEUE_H
