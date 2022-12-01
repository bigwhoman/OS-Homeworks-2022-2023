#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
    int done;
    long number;
} my_message;

extern long fun(long a, long b);

int main() {
    int num_of_children = 8;
    int child_id = 1;

    char* fifo_pipe = "./fifoFile";
    mkfifo(fifo_pipe, 0666);
    int parent_pipe_fd[2];
    int child_pipe_fd[2];
    pipe(parent_pipe_fd);
    pipe(child_pipe_fd);
    for (int i = 0; i < num_of_children; i++) {

        child_id = fork();
        if (child_id == -1) {
            printf("problem in allocating !!!");
            return 1;
        } else if (child_id == 0)
            break;
    }


    if (child_id == 0) {
        long child_ans = 0;
        ssize_t t;
        while (1) {
            my_message got_message;
            if ((t = read(parent_pipe_fd[0], &got_message, sizeof(got_message))) < 0)
                break;
            if (got_message.done == 1)
                break;
            child_ans = fun(child_ans, got_message.number);
            my_message send_message;
            send_message.done = 0;
            send_message.number = child_ans;
            write(child_pipe_fd[1], &send_message, sizeof(send_message));
        }
        my_message send_message;
        send_message.number = child_ans;
        send_message.done = 1;


        int fdj = open(fifo_pipe,O_WRONLY);
        write(fdj, &send_message, sizeof(send_message));
        close(fdj);
        _exit(0);

    } else {
        int number_of_lines = 0;



        /////////////////////////////////////////////////// Read test data
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t readl;

        fp = fopen("./test.txt", "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);
        while ((readl = getline(&line, &len, fp)) != -1) {
            number_of_lines += 1;
        }
        long nums[number_of_lines];
        int line_num = 0;
        long number;
        rewind(fp);
        while ((readl = fscanf(fp, "%ld", &number)) != -1) {
            nums[line_num] = number;
            line_num++;
        }
        fclose(fp);
        if (line)
            free(line);
        int ptr = 0;
        long soom = 0;
        for (int i = 0; i < number_of_lines; ++i) {
            soom += nums[i];
        }
        //////////////////////////////////////////////////////////////




        for (int i = 0; i < 3500 && i < number_of_lines; ++i) {
            my_message message = {getpid(), nums[ptr]};
            write(parent_pipe_fd[1], &message, sizeof(message));
            ptr++;
        }


        long child_answers[num_of_children];


        while (1) {
            if (ptr >= number_of_lines) {
                sleep(1);
                my_message last_message = {1, 0};
                for (int i = 0; i < num_of_children; ++i) {
                    write(parent_pipe_fd[1], &last_message, sizeof(last_message));
                }

                int fdk = open(fifo_pipe,O_RDONLY);
                for (int i = 0; i < num_of_children; ++i) {
                    read(fdk, &last_message, sizeof(last_message));
                    child_answers[i] = last_message.number;
                }
                close(fdk);
                close(child_pipe_fd[0]);
                close(parent_pipe_fd[1]);
                close(child_pipe_fd[1]);
                close(parent_pipe_fd[0]);
                break;
            }
            my_message got_message;
            read(child_pipe_fd[0], &got_message, sizeof(got_message));
            my_message message = {0, nums[ptr]};
            write(parent_pipe_fd[1], &message, sizeof(message));
            ptr++;
        }

        long final_ans = 0;
        for (int i = 0; i < num_of_children; ++i) {
            final_ans = fun(final_ans, child_answers[i]);
        }

        printf("final answer = %ld", final_ans);
    }
    return 0;
}