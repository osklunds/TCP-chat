
#ifndef LIST_H
#define LIST_H

struct node {
    int value;
    struct node *next;
};

struct list {
    struct node *head;  
};

struct list create_list();
void append(struct list *self, int value);
int contains(struct list *self, int value);
int size(struct list *self);
int element_at(struct list *self, int index);
void delete_element_at(struct list *self, int index);

#endif // Header guard