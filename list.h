
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
void insert(struct list *self, int value);
int contains(struct list *self, int value);
int size(struct list *self);

#endif // Header guard