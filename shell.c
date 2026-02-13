#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


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
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return i;
}


int main() {
    printf("Welcome to stupid shell(it's very stupid)\n");
    char *args[64];
    char input[128];
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    while (1) {
    printf("\x1b[33m%s\x1b[0m@\x1b[32m%s\x1b[0m> ", getlogin(), pwd);
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
            fprintf(stderr, "cd: missing argument\n");
        } else if(args[1] == "..") {
            chdir("..");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            chdir(args[1]);
            getcwd(pwd, sizeof(pwd));
        }
        
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            exec_process(args[0], args);
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}

    return 0;
}

