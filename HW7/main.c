#include <stdio.h>
#include "linked_list.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    LinkedList_t *list = (LinkedList_t *) calloc(1, sizeof(LinkedList_t));

    Element_t *a = (Element_t *) calloc(1, sizeof(LinkedList_t));
    a->value = (char *) calloc(10, sizeof(char));
    a->value = "vc";
    Element_t *b = (Element_t *) calloc(1, sizeof(LinkedList_t));
    b->value = (char *) calloc(10, sizeof(char));
    b->value = "gc";
    Element_t *c = (Element_t *) calloc(1, sizeof(LinkedList_t));
    c->value = (char *) calloc(10, sizeof(char));
    c->value = "asdf";
    Element_t *d = (Element_t *) calloc(1, sizeof(LinkedList_t));
    d->value = (char *) calloc(10, sizeof(char));
    d->value = "dddd";
//    Element_t* b = {"cv"};
//    Element_t* c = {"dv"};
    insert(list, a);
    printf("%d\n", get_length(list));
    insert(list, b);
    insert(list, c);
    printf("%d\n", get_length(list));
    insert(list, d);
    printf("%d\n", get_length(list));
    delete(c);
    printf("%d\n", get_length(list));
    Element_t *x = lookup(list, "gc");
    Element_t *t = lookup(list, "kk");
    return 0;
}
