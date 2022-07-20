#include <stdio.h>
#include <stdlib.h>

#include "queue_lib.h"


queue_t* queue_init(int capacity) {
    queue_t *queue = malloc(sizeof(queue_t));
    queue->capacity = capacity;
    queue->items = malloc(sizeof(int)*capacity);
    for (int i = 0; i < capacity; ++i)
        queue->items[i] = -1;
    queue->size = 0;
    queue->p_pop = 0;
    return queue;
}

void queue_delete(queue_t *queue) {
    free(queue->items);
    free(queue);
}

void queue_replace(queue_t* queue) {
    for (int i = queue->capacity; i > 0; i--) {
        queue->items[i] = queue->items[i-1];
    }
    queue->items[0] = -1;
}

void queue_push(queue_t *queue, int data) {
    if (queue->size >= queue->capacity) queue->size = 0;
    if (queue->size != 0)
        queue_replace(queue);
    queue->items[0] = data;
    queue->size++;
}

int queue_pop(queue_t *queue) {
    if (queue->size > 0) {
        queue->size--;
        return queue->items[queue->size];
    } else return -1;
}



void queue_print(queue_t *queue) {
    for (int i = 0; i <  queue->size; i++)
        printf("%d ", queue->items[i]);
    printf("\n");
}

int queue_is_empty(queue_t *queue) {
 if (queue->size == 0) return 1;
 else return 0;
}

int in_queue(queue_t *queue, int el) {
    for (int i = 0; i < queue->capacity; i++)
        if (queue->items[i] == el) return 1;
    return 0;
}