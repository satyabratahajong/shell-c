#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            // Print everything after "echo " followed by a newline
            printf("%s\n", input + 5);
            continue;
        }

        // Step 5: Unrecognized commands
        printf("%s: command not found\n", input);
    }

    return 0;
}