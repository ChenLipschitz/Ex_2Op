#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_ARGS 256
#define MAX_CMD_LEN 1024

void sigint_handler(int sig) {
    write(STDOUT_FILENO, "\n", 1);
}

void execute_cmd(char **args, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

int main() {
    signal(SIGINT, sigint_handler);

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }
        size_t cmd_len = strlen(cmd);
        if (cmd[cmd_len - 1] == '\n') {
            cmd[cmd_len - 1] = '\0';
        }

        char *args[MAX_ARGS];
        int arg_count = 0;
        char *token = strtok(cmd, " ");
        int input_fd = STDIN_FILENO;
        int output_fd = STDOUT_FILENO;
        while (token != NULL) {
            if (strcmp(token, "|") == 0) {
                int pipe_fds[2];
                if (pipe(pipe_fds) < 0) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                execute_cmd(args, input_fd, pipe_fds[1]);
                close(pipe_fds[1]);
                input_fd = pipe_fds[0];
                arg_count = 0;
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " ");
                output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (output_fd < 0) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " ");
                output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (output_fd < 0) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(token, "exit") == 0) {
                exit(EXIT_SUCCESS);
            } else {
                args[arg_count++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        execute_cmd(args, input_fd, output_fd);
    }

    return 0;
}
