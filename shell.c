#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


char pwd[1024];
pid_t pid;
int process_running = 0;


int exec_process(char *command, char *arguments[]) {

    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, arguments);
        perror("exec failed");
        exit(127);
    } else if (pid > 0) {
        process_running = 1;
        int status;
        waitpid(pid, &status, 0);
        process_running = 0;
        return WEXITSTATUS(status);
    } else {
        perror("fork failed");
        return 1;
    }
}

int parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return i;
}

void handle_term(int signal){
    if (process_running == 0){
    getcwd(pwd, sizeof(pwd));
    if (strcmp(pwd, getenv("HOME")) != 0)
    printf("\n\x1b[33m%s\x1b[0m@\x1b[32m%s\x1b[0m$ ", getlogin(), pwd);
    else
    printf("\n\x1b[33m%s\x1b[0m@\x1b[32m~\x1b[0m$ ", getlogin());
    fflush(stdout);
    }
}


int main() {
    signal(SIGINT, handle_term);
    printf("Welcome to stupid shell(it's very stupid)\n");
    char *args[64];
    char input[128];
    chdir(getenv("HOME"));
    getcwd(pwd, sizeof(pwd));
    while (1) {
    if (strcmp(pwd, getenv("HOME")) != 0)
    printf("\x1b[33m%s\x1b[0m@\x1b[32m%s\x1b[0m$ ", getlogin(), pwd);
    else
    printf("\x1b[33m%s\x1b[0m@\x1b[32m~\x1b[0m$ ", getlogin());
    fgets(input, sizeof(input), stdin);
    parse_input(input, args);
    if (args[0] == NULL){
        continue;
    }
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[2] != NULL)
        {
            fprintf(stderr, "cd: too many arguments\n");
        } else if (args[1] == NULL) {
            chdir(getenv("HOME"));
            getcwd(pwd, sizeof(pwd));
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            getcwd(pwd, sizeof(pwd));
        }
    } else if (strcmp(args[0], "exec") == 0) {
        execvp(args[1], &args[1]);
    } else {
        exec_process(args[0], args);
    }
}

    return 0;
}

