#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>

typedef struct task task;
struct task {
    int m;
    int n;
    int priority;
    long taskId;
};

pthread_t *thread;
int *thread_busy;
struct task *tasks;

int getInputs();

int num_of_tasks;
int worker_return = 0;
int task_ptr = 0;
int *master_signal;
int available_threads;

long long Ackerman(int m, int n) {
    long long ans = 0;
    if (m == 0)
        ans = n + 1;
    else if (n == 0)
        ans = Ackerman(m - 1, 1);
    else if (m > 0 && n > 0)
        ans = Ackerman(m - 1, Ackerman(m, n - 1));
    return ans;
}

void *workerThread(void *vargp) {
    int worker_num = (int) (size_t) vargp;
    while (!worker_return) {
        while (!master_signal[worker_num]) {
            if (worker_return)
                return NULL;
        }
        if (worker_return)
            return NULL;
        master_signal[worker_num] = 0;
        thread_busy[worker_num] = 1;
        task t = tasks[task_ptr];
        long long ans = Ackerman(t.m, t.n);
        printf("A(%d,%d) = %llu\n", t.m, t.n, ans);
        thread_busy[worker_num] = 0;
    }
    return NULL;
}

void *masterThread(void *vargp) {
    while (task_ptr < num_of_tasks) {
        for (int i = 0; i < available_threads; i++) {
            if (thread_busy[i] == 0) {
                master_signal[i] = 1;
                usleep(10000);
                task_ptr++;
                if (task_ptr >= num_of_tasks)
                    break;
            }
        }
    }
    usleep(10000);
    worker_return = 1;
    return NULL;
}

int compare(const void *a, const void *b) {
    struct task task1 = *((task *) a);
    struct task task2 = *((task *) b);
    return (task1.priority - task2.priority);
}


int main() {
    pthread_t tid;
    pthread_t master;
    int k = getInputs();

    pthread_create(&master, NULL, masterThread, (void *) NULL);
    for (int i = 0; i < k; i++) {
        pthread_create(&tid, NULL, workerThread, (void *) (size_t) i);
        thread[i] = tid;
    }


    int s = pthread_join(master, NULL);
    if (s != 0)
        exit(1);
    for (int i = 0; i < k; i++) {
        s = pthread_join(thread[i], NULL);
        if (s != 0)
            exit(1);
    }


    free(thread);
    free(tasks);
    free(thread_busy);
    free(master_signal);
    return 0;
}

int getInputs() {
    int k = 4;
    available_threads = k;
    printf("input number of tasks : ");
    num_of_tasks;
    scanf("%d", &num_of_tasks);

    {
        tasks = malloc(num_of_tasks * sizeof(task));
    }
    for (int i = 0; i < num_of_tasks; i++) {
        int n, m;
        scanf("%d %d", &m, &n);
        task t = {m, n, m + n, random()};
        tasks[i] = t;
    }

    qsort(tasks, num_of_tasks, sizeof(task), compare);

    pthread_t tid;
    pthread_t master;
    {
        thread = malloc(k * sizeof(pthread_t));
        thread_busy = calloc(k, sizeof(int));
        master_signal = calloc(k, sizeof(int));
    }
    return k;
}
