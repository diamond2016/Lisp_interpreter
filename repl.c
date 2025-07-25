#include <stdio.h>
#include <stdlib.h>
#include "linenoise/linenoise.h"

#define HISTORY_LEN   1024

int main(int argc, char** argv) {

  /* Print Version and Exit Information */
  puts("Lispy Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  if (linenoiseHistorySetMaxLen(HISTORY_LEN) == 0) {
      fprintf(stderr, "could not set history buffer, exiting...");
      exit(1);
  }

  /* In a never ending loop */
  while (1) {
    char *line = NULL;

    /* Output our prompt and get input */
    if ((line = linenoise("lispy> ")) == NULL) {
      fprintf(stderr, "could not read input line, exiting...");
      exit(1);
    }

    /* Add input to history */
    if ((linenoiseHistoryAdd(line)) == 0) {
      fprintf(stderr, "could not add line to history, exiting...");
      exit(1);
    };

    /* Echo input back to user */
    printf("No you're a %s\n", line);

    /* Free retrieved input */
    linenoiseFree(line);

  }

  return 0;
}