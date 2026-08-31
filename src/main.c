#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *VALID_COMMANDS[3] = {"type", "exit", "echo"};
const int NUM_COMMANDS = 3;

void handleType(char *buf) {
  char *path = strdup(getenv("PATH"));
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (strcmp(buf, VALID_COMMANDS[i]) == 0) {
      printf("%s is a shell builtin\n", buf);
      return;
    }
  }
  for (char *p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":")) {
    char fp[1024];
    sprintf(fp, "%s/%s", p, buf);
    if (access(fp, X_OK) == 0) {
      printf("%s is %s\n", buf, fp);
      return;
    }
  }
  printf("%s: not found\n", buf);
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  char buf[1024];
  while (1) {
    printf("$ ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strcmp(buf, "exit") == 0)
      break;
    else if (strncmp(buf, "echo ", 5) == 0)
      printf("%s\n", buf + 5);
    else if (strncmp(buf, "type ", 5) == 0)
      handleType(buf + 5);
    else
      printf("%s: command not found\n", buf);
  }
  return 0;
}