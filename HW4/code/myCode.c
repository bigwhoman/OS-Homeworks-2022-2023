#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>

// Let us create a global variable to change it in threads
int g = 0;
int **matrix_p;
int matrix_size;
int wait = 0;
static pthread_barrier_t barrier;
#define INFINITY INT_MAX

// The function to be executed by all threads
void *myThreadFun(void *vargp) {
    // Store the value argument passed to this thread
    int *myid = (int *) vargp;
    int first_id = myid[0];
    int second_id = myid[1];
    for (int i = 0; i < matrix_size; i++) {
        int min_arg = matrix_p[first_id][second_id];

        if (first_id != second_id)
            for (int j = 0; j < matrix_size; j++) {
                int said_min_arg = INT_MAX;
                if (matrix_p[first_id][j] == -1 || matrix_p[j][second_id] == -1) {
                    said_min_arg = -1;
                }
                if (said_min_arg != -1) {
                    said_min_arg = matrix_p[first_id][j] + matrix_p[j][second_id];
                    if (said_min_arg < min_arg || min_arg == -1) {
                        min_arg = said_min_arg;
                    }
                }
            }
        int s = pthread_barrier_wait(&barrier);
        matrix_p[first_id][second_id] = min_arg;
    }
    return NULL;
}

int main() {
    printf("enter matrix size : ");
    scanf("%d",&matrix_size);
    {
        matrix_p = malloc(matrix_size * sizeof(int *));
        for (int i = 0; i < matrix_size; i++)
            matrix_p[i] = malloc(matrix_size * sizeof(int));
    }
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("./testcase.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char x[1024];
    int row = 0;
    int column = 0;
    /* assumes no word exceeds length of 1023 */
    while (fscanf(fp, "%1023s", x) == 1) {

        if (!strcmp("∞", x))
            matrix_p[row][column] = -1;
        else if (strcmp(" ", x) != 0) {
            int num;
            sscanf(x, "%d", &num);
            matrix_p[row][column] = num;
        }
        column++;
        if (column == matrix_size) {
            if (row == matrix_size - 1)
                break;
            column = 0;
            row++;
        }
    }
    fclose(fp);

    int i;


    int s = pthread_barrier_init(&barrier, NULL, matrix_size * matrix_size);
    if (s != 0)
        exit(1);

    pthread_t tid;
    pthread_t threads[matrix_size][matrix_size];
    for (i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; ++j) {
            int *n = malloc(2 * sizeof(int));
            n[0] = i;
            n[1] = j;
            pthread_create(&tid, NULL, myThreadFun, (void *) n);
            threads[i][j] = tid;
        }
    for (i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; ++j) {
            s = pthread_join(threads[i][j], NULL);
            if (s != 0)
                exit(1);
        }


    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; ++j) {
            if (matrix_p[i][j] == -1)
                printf("∞ ");
            else
                printf("%d ", matrix_p[i][j]);
        }
        printf("\n");
    }

    return 0;
}
