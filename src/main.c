#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Disable output buffering so prompts print immediately
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    char input[1024];

    while (1) {
        // Step 1: Print the prompt
        printf("$ ");

        // Step 2: Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit loop on EOF
        }

        // Remove trailing newline character (\n)
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty input lines
        if (strlen(input) == 0) {
            continue;
        }

        // Step 3: Check for the 'exit' builtin command
        if (strcmp(input, "exit") == 0 || strcmp(input, "exit 0") == 0) {
            return 0; // Terminate the shell with exit code 0
        }

        // Step 4: Handle unrecognized commands
        printf("%s: command not found\n", input);
    }

    return 0;
}