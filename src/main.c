#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Helper array to check supported shell builtins
const char *builtins[] = {"echo", "exit", "type", NULL};

// Check if a given command name is a builtin
int is_builtin(const char *cmd) {
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Search for command in the directories listed in the PATH environment variable.
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

        // Check if file exists and has execute permissions
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }

        dir = strtok(NULL, delimiter);
    }

    free(path_copy);
    return NULL;
}