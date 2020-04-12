
#include <stdlib.h>

#include "list.h"


struct node *create_node() {
    return calloc(1, sizeof(struct node));
}

struct list create_list() {
    struct list lst;

    lst.head = NULL;

    return lst;
}

void insert(struct list *self, int value) {
    if (self->head == NULL) {
        self->head = create_node();
        self->head->value = value;
    } else {
        struct node *current_node = self->head;

        while (current_node->next != NULL) {
            current_node = current_node->next;
        }

        current_node->next = create_node();
        current_node = current_node->next;
        current_node->value = value;
    }
}

int contains(struct list *self, int value) {
    struct node *current_node = self->head;

    while (current_node != NULL) {
        if (current_node->value == value) {
            return 1;
        }

        current_node = current_node->next;
    }

    return 0;
}

int size(struct list *self) {
    int size = 0;
    struct node *current_node = self->head;

    while (current_node != NULL) {
        size++;
        current_node = current_node->next;
    }


    return size;
}

