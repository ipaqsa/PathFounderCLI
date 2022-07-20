#ifndef BASESTRUCTURE_QUEUE_LIB_H
#define BASESTRUCTURE_QUEUE_LIB_H

typedef struct {
    int capacity;
    int *items;
    int size;
    int p_pop;
} queue_t;

queue_t* queue_init(int capacity);
void queue_delete(queue_t *queue);
void queue_push(queue_t *queue, int data);
int queue_pop(queue_t *queue);
void queue_print(queue_t *queue);
void queue_replace(queue_t *queue);
int queue_is_empty(queue_t *queue);
int queue_front(queue_t *queue);
int in_queue(queue_t *queue, int el);
#endif
