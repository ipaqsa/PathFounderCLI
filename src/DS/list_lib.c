#include <stdio.h>
#include <stdlib.h>

#include "list_lib.h"

list_t* list_init(int data) {
    list_t *list = malloc(sizeof(list_t));
    list->data = data;
    list->next = NULL;
    list->prev = NULL;
    list->idx = 0;
    return list;
}

void list_add_to_end(list_t *list, int data) {
    list_t *last;
    last = list;
    while (last->next != NULL)
        last = last->next;

    list_t *node = malloc(sizeof(list_t));
    last->next = node;

    node->idx = last->idx+1;
    node->data = data;
    node->next = NULL;
    node->prev = last;
}

void list_add_to_begin(list_t *list, int data) {
    list_t* node = malloc(sizeof(list_t));
    int tmp_data = list->data;
    int tmp_idx = list->idx;
    list_t* tmp_next = list->next;

    list->data = data;
    list->next = node;
    list->prev = NULL;
    list->idx = tmp_idx+1;

    node->prev = list;
    node->idx = tmp_idx;
    node->data = tmp_data;
    node->next = tmp_next;
}

void list_add(list_t* list, int data, int flag, int re) {
    if (flag == 1) list_add_to_begin(list, data);
    else if (flag == 0) list_add_to_end(list, data);
    if (flag == 1 && re == 1) list_last_delete(list);
    if (flag == 0 && re == 1) list_first_delete(list);
}

void list_node_insert(list_t *list, int idx, int data) {
    int count = 0;
    int tmp_data = 0;
    list_t *tmp_next = NULL;
    list_t *_list;
    _list = list;
    while (_list->next != NULL) {
        count++;
        _list = _list->next;
        if (count == idx)
            break;
    }
    list_t *node = malloc(sizeof(list_t));
    tmp_data = _list->data;
    tmp_next = _list->next;

    _list->data = data;
    _list->next = node;

    node->data = tmp_data;
    node->next = tmp_next;
}

void list_first_delete(list_t *list) {
    int tmp_data;
    list_t *tmp_next;

    list_t *second = list->next;
    tmp_data = second->data;
    tmp_next = second->next;

    list->data = tmp_data;
    list->next = tmp_next;
    free(second);
}
//void list_node_delete(list_t *list, int idx) {
//    list_t *tmp;
//    int tmp_data;
//    list_t *tmp_next;
//    tmp = list;
//    int count = 0;
//    while (tmp->next != NULL) {
//        count++;
//        tmp = tmp->next;
//        if (count == idx-1) break;
//    }
//    tmp_data = tmp->next->data;
//    tmp_next = tmp->next->next;
//
//};

int list_data(list_t *list, int idx) {
    list_t *tmp;
    tmp = list;
    int count = 0;
    while (count != idx) {
        if (tmp->next != NULL) {
            tmp = tmp->next;
            count++;
        }
        else return -1;
    }
    return tmp->data;
}

int list_size(list_t *list) {
    list_t *tmp;
    tmp = list;
    int count = 1;
    while (tmp->next != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}


void list_print(list_t *list) {
    list_t *tmp;
    tmp = list;
    printf("%d ", tmp->data);
    while (tmp->next != NULL) {
        tmp = tmp->next;
        printf("%d ", tmp->data);
    }
}

int in_list(list_t* list, int element) {
    list_t *tmp = list;
    if (tmp->data == element) return 1;
    while (1) {
        if (tmp->data == element) return 1;
        if (tmp->next == NULL) return 0;
        tmp = tmp->next;
    }
}

void list_delete(list_t *list) {
    list_t *tmp;
    list_t *last;
    last = list;
    while (last->next != NULL) {
        tmp = last;
        last = last->next;
        free(tmp);
    }
};


void list_last_delete(list_t *list) {
    int size = list_size(list);
    list_t *tmp_node;
    tmp_node = list;
    int count = 1;
    while (tmp_node->next != NULL) {
        count++;
        tmp_node = tmp_node->next;
        if (count == size-1) break;
    }
    free(tmp_node->next);
    tmp_node->next = NULL;
}

int get_node_idx(const list_t* list) {
    return list->idx;
}

int get_node_data(const list_t* list) {
    return list->data;
}

void next_node(list_t** list) {
    if ((*list)->next != NULL)
        *list = (*list)->next;
}

void prev_node(list_t** list) {
    if ((*list)->prev != NULL)
        *list = (*list)->prev;
}

void list_copy(list_t* list, list_t** copy) {
    *copy = list_init(0);
    list_t* tmp = list;
    if (list == NULL) return;
    int state = 1;
    while (1) {
        list_add(*copy, tmp->data, 0, state);
        if (state) state = 0;
        if (tmp->next == NULL) return;
        tmp = tmp->next;
    }
}
void list_reverse(list_t** list) {
    list_t* tmp = *list;
    list_t* copy = NULL;
    list_t* result = list_init(0);
    list_copy(*list, &copy);
    while (1) {
        if (tmp->next == NULL) break;
        next_node(&tmp);
    }
    while (1) {
        list_add_to_end(result, tmp->data);
        if (tmp->prev == NULL) break;
        prev_node(&tmp);
    }
    list_first_delete(result);
    list_delete(*list);
    *list = result;
}

list_t* list_slice(list_t* list, int begin, int end) {
    if (begin > end) begin = 0;
    if (end > list_size(list)) end = list_size(list);
    if (begin == end || begin > list_size(list)) begin = end - 1;
    if (begin < 0) begin = 0;
    list_t* result = list_init(0);
    list_t* tmp = list;
    while (tmp->idx != begin) next_node(&tmp);
    while (1) {
        list_add_to_end(result, tmp->data);
        if (tmp->idx == end) break;
        if (tmp->next == NULL) break;
        next_node(&tmp);
    }
    list_first_delete(result);
    return result;
}