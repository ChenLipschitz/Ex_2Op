#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

//ignore the command ^C
void signal_handler() {
    printf("\n");
}

//commands executer
void cmd(int inputF, int outputF, char** args) {
    int p_id = fork();
    switch (p_id) {
        case -1:
            fprintf(stderr, "fork: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
            break;
        case 0:
            if (inputF != STDIN_FILENO) {
                dup2(inputF, STDIN_FILENO);
                close(inputF);
            }
            if (outputF != STDOUT_FILENO) {
                dup2(outputF, STDOUT_FILENO);
                close(outputF);
            }
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "execvp: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        default:
            wait(NULL);
            break;
    }
}

//input parser
void parser(char* command, char** args, int* argCount, int* inputF, int* outputF) {
    char *token = strtok(command, " ");
    *argCount = 0;
    *inputF = STDIN_FILENO;
    *outputF = STDOUT_FILENO;

    while (token != NULL) {
        if (strcmp(token, "|") == 0) {
            int piping[2];
            if (pipe(piping) < 0) {
                fprintf(stderr, "pipe: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            cmd(*inputF, piping[1], args);
            close(piping[1]);
            *inputF = piping[0];
            *argCount = 0;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            *outputF = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (*outputF < 0) {
                fprintf(stderr, "open: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            *outputF = open(token, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (*outputF < 0) {
                fprintf(stderr, "open: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, "exit") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            args[(*argCount)++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[*argCount] = NULL;
}


int main() {
    signal(SIGINT, signal_handler);
    int outputF;
    int inputF;
    char *args[256];
    int argCount;
    char command[1024];
    
    while (1) {
        fflush(stdout);
        if (fgets(command, 1024, stdin) == NULL) {
            break;
        }
        
        //from basic shell
        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }
        
        parser(command, args, &argCount, &inputF, &outputF);
        cmd(inputF, outputF, args);
    }

    return 0;
}
