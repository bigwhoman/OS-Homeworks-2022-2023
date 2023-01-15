#include <stdio.h>
#include "linked_list.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t *thread;
int num_iterations;
pthread_mutex_t lock;
pthread_mutex_t lock2;
LinkedList_t *list;
FILE *result;

int parse_input(const char *arg, char eq);

void print_list();

void *workerThread() {
    ssize_t readl;
    char *str = calloc(100, sizeof(char));
    sprintf(str, "%lu", pthread_self() % 10000);
    for (int i = 0; i < num_iterations; i++) {
        char x = 'a';
        str = strncat(str, &x, 1);
        Element_t *a = (Element_t *) calloc(1, sizeof(Element_t));
        a->value = (char *) calloc(100 * 20, 200 * sizeof(char));
        a->value = str;
        pthread_mutex_lock(&lock);
        insert(list, a);
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock2);
        fprintf(result, "Log(insert(x)) -> thread %lu inserted %s \n", pthread_self(), str);
        pthread_mutex_unlock(&lock2);
//        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock2);
        fprintf(result,
                "Log(get_length(x)) -> thread %lu : length is %d\n", pthread_self(), get_length(list));
        pthread_mutex_unlock(&lock2);

        pthread_mutex_lock(&lock2);
        fprintf(result, "Log(lookup(x)) -> thread %lu found %s \n",
                pthread_self(), lookup(list, str)->value);
        pthread_mutex_unlock(&lock2);
        pthread_mutex_lock(&lock);
        delete(a);
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&lock2);
        fprintf(result, "Log(delete(x)) -> thread %lu deleted %s \n", pthread_self(), str);
        pthread_mutex_unlock(&lock2);

        
    }
    free(str);
    return NULL;
}

int main(int argc, char *argv[]) {
    char *arg1 = argv[1];
    char *arg2 = argv[2];
    char eq = '=';
    int num_threads = parse_input(arg1, eq);
    num_iterations = parse_input(arg2, eq);
    list = (LinkedList_t *) calloc(1, sizeof(LinkedList_t));
    result = fopen("./outputs/result.txt", "w+");

    Element_t *a = (Element_t *) calloc(1, sizeof(LinkedList_t));
    a->value = (char *) calloc(100 * 20, 200 * sizeof(char));
    a->value = "vc";
    Element_t *b = (Element_t *) calloc(1, sizeof(LinkedList_t));
    b->value = (char *) calloc(100 * 20, 200 * sizeof(char));
    b->value = "gc";
    Element_t *c = (Element_t *) calloc(1, sizeof(LinkedList_t));
    c->value = (char *) calloc(100 * 20, 200 * sizeof(char));
    c->value = "asdf";
    Element_t *d = (Element_t *) calloc(1, sizeof(LinkedList_t));
    d->value = (char *) calloc(100 * 20, 200 * sizeof(char));
    d->value = "dddd";
    insert(list, a);
    insert(list, b);
    insert(list, d);
    print_list();

    pthread_t tid;
    {
        thread = malloc(num_threads * sizeof(pthread_t));
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&tid, NULL, workerThread, NULL);
        thread[i] = tid;
    }

    for (int i = 0; i < num_threads; i++) {
        int s = pthread_join(thread[i], NULL);
        if (s != 0)
            exit(1);
    }
    fclose(result);
    print_list();
    return 0;
}


void print_list() {
    Element_t *start = list->next;
    if (start == NULL) {
        printf("NO ITEMS IN LIST \n");
        return;
    }
    printf("------------------------ list ------------------------------\n");
    while (start != list) {
        printf("item with pointer number : %u and value : %s \n", (int) (size_t) start, start->value);
        start = start->next;
    }
    printf("------------------------ end ------------------------------\n");
}

int parse_input(const char *arg, char eq) {
    char *quotPtr = strchr(arg, eq);
    if (quotPtr == NULL) {
    }
    int position = quotPtr - arg;
    char *attrValue = (char *) malloc((position + 1) * sizeof(char));
    memcpy(attrValue, arg, position);
    attrValue[position] = '\0';
    char *t = calloc(10, sizeof(char));
    char x = arg[position + 1];
    while (x != '\0') {
        t = strncat(t, &x, 1);
        position++;
        x = arg[position + 1];
    }
    int num = atoi(t);
    return num;
}
