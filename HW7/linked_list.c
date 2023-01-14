#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

pthread_mutex_t lock;

void insert(LinkedList_t *list, Element_t *element) {
    pthread_mutex_lock(&lock);
    if (list->prev != NULL) {
        (list->prev)->next = element;
        element->prev = (list->prev);
        (list->prev) = element;
    } else {
        list->next = element;
        list->prev = element;
        element->next = list;
        element->prev = list;
    }
    pthread_mutex_unlock(&lock);
}

int delete(Element_t *element) {
    pthread_mutex_lock(&lock);
    if (element->value == NULL) {
        pthread_mutex_unlock(&lock);
        return 1;
    } else {
        (element->next)->prev = element->prev;
        (element->prev)->next = element->next;
        free(element);
        pthread_mutex_unlock(&lock);
        return 0;
    }
}

Element_t *lookup(LinkedList_t *list, const char *value) {
    pthread_mutex_lock(&lock);
    if (list->next == NULL || list->next == list) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }
    Element_t *found = list->next;
    while (found != list->prev) {
        if (found->value == value) {
            pthread_mutex_unlock(&lock);
            return found;
        }
        found = found->next;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int get_length(LinkedList_t *list) {
    pthread_mutex_lock(&lock);
    int ans = 0;
    if (list->next == NULL || list->next == list) {
        pthread_mutex_unlock(&lock);
        return 0;
    }
    Element_t *lili = list->next;
    while (lili != list->prev) {
        ans++;
        lili = lili->next;
    }
    ans++;
    pthread_mutex_unlock(&lock);
    return ans;
}
