#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Disable output buffering so prompts and messages print immediately
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    char input[1024];

    while (1) {
        // Step 1: Print the prompt
        printf("$ ");

        // Step 2: Read input from standard input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit loop on EOF (Ctrl+D / end of pipe)
        }

        // Remove trailing newline character (\n)
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty input lines (pressing enter on empty prompt)
        if (strlen(input) == 0) {
            continue;
        }

        // Step 3: Print command not found error message
        printf("%s: command not found\n", input);
    }

    return 0;
}