#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <io.h>
  #include <process.h>
  #define access _access
  #define X_OK 0  // Check existence on Windows
#else
  #include <unistd.h>
  #include <sys/wait.h>
#endif

const char *builtins[] = {"echo", "exit", "type", NULL};

int is_builtin(const char *cmd) {
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *get_executable_path(const char *cmd) {
    char *path_env = getenv("PATH");
    if (!path_env) {
        return NULL;
    }

    char *path_copy = strdup(path_env);
    if (!path_copy) {
        return NULL;
    }

    #ifdef _WIN32
    const char *delimiter = ";";
    #else
    const char *delimiter = ":";
    #endif

    char *dir = strtok(path_copy, delimiter);
    static char full_path[1024];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }

        dir = strtok(NULL, delimiter);
    }

    free(path_copy);
    return NULL;
}

int main(void) {
    // Disable output buffering so prompts print immediately
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    char input[1024];

    while (1) {
        // Step 1: Print prompt
        printf("$ ");

        // Step 2: Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF
        }

        // Remove trailing newline character (\n)
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty input lines
        if (strlen(input) == 0) {
            continue;
        }

        // Step 3: Handle 'exit' builtin
        if (strcmp(input, "exit") == 0 || strcmp(input, "exit 0") == 0) {
            return 0;
        }

        // Step 4: Handle 'echo' builtin
        if (strcmp(input, "echo") == 0) {
            printf("\n");
            continue;
        } else if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
            continue;
        }

        // Step 5: Handle 'type' builtin
        if (strncmp(input, "type ", 5) == 0) {
            char *arg = input + 5;

            if (is_builtin(arg)) {
                printf("%s is a shell builtin\n", arg);
            } else {
                char *exec_path = get_executable_path(arg);
                if (exec_path != NULL) {
                    printf("%s is %s\n", arg, exec_path);
                } else {
                    printf("%s: not found\n", arg);
                }
            }
            continue;
        }

        // Step 6: Parse arguments for external commands
        char *args[64];
        int argc = 0;

        char *token = strtok(input, " ");
        while (token != NULL && argc < 63) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL; // NULL-terminate array

        if (argc == 0) {
            continue;
        }

        char *cmd = args[0];

        // Step 7: Search for command executable path
        char *exec_path = get_executable_path(cmd);

        if (exec_path != NULL) {
            char path_buf[1024];
            strncpy(path_buf, exec_path, sizeof(path_buf));

#ifdef _WIN32
            // Windows execution using spawnvp
            intptr_t ret = _spawnvp(_P_WAIT, path_buf, (const char * const *)args);
            if (ret == -1) {
                perror("_spawnvp failed");
            }
#else
            // POSIX execution using fork and execv
            pid_t pid = fork();
            if (pid == 0) {
                execv(path_buf, args);
                perror("execv failed");
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork failed");
            }
#endif
        } else {
            printf("%s: command not found\n", cmd);
        }
    }

    return 0;
}