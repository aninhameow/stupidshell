#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int exec_process(char *command, char *arguments[]) {

    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, arguments);
        perror("exec failed");
        exit(127);
    } else if (pid > 0) {
        // Parent
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        perror("fork failed");
        return 1;
    }
}

int parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n"); // split on space, tab, newline
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // null-terminate for execvp
    return i;       // number of arguments
}

int main() {
    char program[128];
    char *args[64];
    char input[128];
    while (1) {
    fgets(input, sizeof(input), stdin);
    parse_input(input, args);
        pid_t pid = fork();
        if (pid == 0) {
            exec_process(args[0], args);
        } else {
            waitpid(pid, NULL, 0);
        }
}

    return 0;
}

