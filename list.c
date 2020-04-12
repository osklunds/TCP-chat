
#include <stdlib.h>

#include "list.h"


struct node *create_node() {
    return calloc(1, sizeof(struct node));
}

void append(struct list *self, int value) {
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

int element_at(struct list *self, int index) {
    struct node *current_node = self->head;

    while (index > 0) {
        index--;
        current_node = current_node->next;
    }

    return current_node->value;
}

void delete_element_at(struct list *self, int index) {
    if (index == 0) {
        self->head = self->head->next;
    } else {
        struct node *current_node = self->head;

        while (index > 1) {
            index--;
            current_node = current_node->next;
        }

        struct node *to_delete = current_node->next;
        struct node *next = to_delete->next;

        current_node->next = next;
        free(to_delete);
    }
}

