#ifndef BASESTRUCTURE_LIST_LIB_H
#define BASESTRUCTURE_LIST_LIB_H

typedef struct node{
    int data;
    int idx;
    struct node *next;
    struct node*prev;
} list_t;


list_t* list_init(int data);
void list_add_to_end(list_t *prev, int data);
void list_add_to_begin(list_t *list, int data);
void list_node_insert(list_t *list, int idx, int data);
int list_data(list_t *first, int idx);
void list_last_delete(list_t *list);
void list_first_delete(list_t *list);
int in_list(list_t* list, int element);
void list_node_delete(list_t *list, int idx);
void list_delete(list_t *list);
void list_print(list_t *list);
int list_size(list_t *list);
int get_node_idx(const list_t* list);
int get_node_data(const list_t* list);
list_t* list_slice(list_t* list, int begin, int end);
void next_node(list_t** list);
void prev_node(list_t** list);
void list_add(list_t* list, int data, int flag, int re);
void list_reverse(list_t** list);
void list_copy(list_t* list, list_t** copy);
#endif
