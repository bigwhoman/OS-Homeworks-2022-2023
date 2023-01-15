#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

void insert(LinkedList_t *list, Element_t *element) {
    if (list->prev != NULL) {
        (list->prev)->next = element;
        element->prev = (list->prev);
        (list->prev) = element;
        element->next = list;
    } else {
        list->next = element;
        list->prev = element;
        element->next = list;
        element->prev = list;
    }
}

int delete(Element_t *element) {
    if (element->value == NULL) {
        return 1;
    } else {
        (element->next)->prev = element->prev;
        (element->prev)->next = element->next;
        free(element);
        return 0;
    }
}

Element_t *lookup(LinkedList_t *list, const char *value) {
    if (list->next == NULL || list->next == list) {
        return NULL;
    }
    Element_t *found = list->next;
    while (found != list) {
        if (strcmp(found->value,value) == 0) {
            return found;
        }
        found = found->next;
    }
    return NULL;
}

int get_length(LinkedList_t *list) {
    int ans = 0;
    if (list->next == NULL || list->next == list) {
        return 0;
    }
    Element_t *lili = list->next;
    while (lili != list->prev) {
        ans++;
        lili = lili->next;
    }
    ans++;
    return ans;
}
