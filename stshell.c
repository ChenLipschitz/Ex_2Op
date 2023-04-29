#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_ARGS 256


// execute a command with input and output redirection
void exec_cmd(char **args, int input_fd, int output_fd) {
    pid_t PID = fork();     //the PID of the child process
    if (PID < 0) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (PID == 0) {
        if (input_fd != STDIN_FILENO) {
            if (dup2(input_fd, STDIN_FILENO) == -1) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            if (dup2(output_fd, STDOUT_FILENO) == -1) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(output_fd);
        }
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "execvp: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    } else {
        if (waitpid(PID, NULL, 0) == -1) {
            fprintf(stderr, "wait: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}


// int main() {
//     signal(SIGINT,signal_handler);

//     char cmd[1024];
//     char *args[MAX_ARGS];
//     int arg_count;
//     char *token;
//     int input_fd;
//     int output_fd;
    
//     while (1) {
//         // printf("> ");
//         fflush(stdout);
//         if (fgets(cmd, 1024, stdin) == NULL) {
//             break;
//         }
        
//         if (cmd[strlen(cmd) - 1] == '\n') {
//             cmd[strlen(cmd) - 1] = '\0';
//         }

//         arg_count = 0;
//         token = strtok(cmd, " ");
//         input_fd = STDIN_FILENO;
//         output_fd = STDOUT_FILENO;
        
//         //command line parser
//         while (token != NULL) {
//             if (strcmp(token, "|") == 0) {
//                 int pipe_fds[2];
//                 if (pipe(pipe_fds) < 0) {
//                     perror("pipe");
//                     exit(EXIT_FAILURE);
//                 }
//                 exec_cmd(args, input_fd, pipe_fds[1]);
//                 close(pipe_fds[1]);
//                 input_fd = pipe_fds[0];
//                 arg_count = 0;
//             } else if (strcmp(token, ">") == 0) {
//                 token = strtok(NULL, " ");
//                 output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
//                 if (output_fd < 0) {
//                     perror("open");
//                     exit(EXIT_FAILURE);
//                 }
//             } else if (strcmp(token, ">>") == 0) {
//                 token = strtok(NULL, " ");
//                 output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0666);
//                 if (output_fd < 0) {
//                     perror("open");
//                     exit(EXIT_FAILURE);
//                 }
//             } else if (strcmp(token, "exit") == 0) {
//                 exit(EXIT_SUCCESS);
//             } else {
//                 args[arg_count++] = token;
//             }
//             token = strtok(NULL, " ");
//         }
//         args[arg_count] = NULL;
//         exec_cmd(args, input_fd, output_fd);
//     }

//     return 0;
// }



void signal_handler(int sig) {
    printf("\n");
}

void parse_input(char* cmd, char** args, int* arg_count, int* input_fd, int* output_fd) {
    char *token = strtok(cmd, " ");
    *arg_count = 0;
    *input_fd = STDIN_FILENO;
    *output_fd = STDOUT_FILENO;

    while (token != NULL) {
        if (strcmp(token, "|") == 0) {
            int pipe_fds[2];
            if (pipe(pipe_fds) < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            exec_cmd(args, *input_fd, pipe_fds[1]);
            close(pipe_fds[1]);
            *input_fd = pipe_fds[0];
            *arg_count = 0;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (*output_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (*output_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, "exit") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            args[(*arg_count)++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[*arg_count] = NULL;
}
void execute_commands(char** args, int input_fd, int output_fd) {
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;
        case 0:
            if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            wait(NULL);
            break;
    }
}

int main() {
    signal(SIGINT, signal_handler);
    char cmd[1024];
    char *args[MAX_ARGS];
    int arg_count;
    int input_fd;
    int output_fd;
    
    while (1) {
        // printf("> ");
        fflush(stdout);
        if (fgets(cmd, 1024, stdin) == NULL) {
            break;
        }
        
        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0';
        }
        
        parse_input(cmd, args, &arg_count, &input_fd, &output_fd);
        execute_commands(args, input_fd, output_fd);
    }

    return 0;
}
