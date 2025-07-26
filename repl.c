#include <stdio.h>
#include <stdlib.h>
#include "linenoise/linenoise.h"
#include "mpc/mpc.h"

#define HISTORY_LEN   1024

int main(int argc, char** argv) {

  /* Print Version and Exit Information */
  puts("Lispy Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  if (linenoiseHistorySetMaxLen(HISTORY_LEN) == 0) {
      fprintf(stderr, "could not set history buffer, exiting...");
      exit(1);
  }

  /* create some parsers */
  mpc_parser_t *Number  = mpc_new("number");
  mpc_parser_t *Operator  = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  /* regex between \/ and with ; at end*/
  mpca_lang(MPCA_LANG_DEFAULT,
    " number : /-?[0-9]+.?[0-9]*/; "
    " operator : \"add\" | \"sub\" | \"mul\" | \"div\"; "
    " expr : <number> | '(' <operator> <expr>+ ')'; "
    " lispy : /^/ <expr>+ /$/; ",
    Number, Operator, Expr, Lispy);

  /* In a never ending loop */
  while (1) {
    char *input = linenoise("lispy> ");

    /* Add input to history */
    if ((linenoiseHistoryAdd(input)) == 0) {
      fprintf(stderr, "could not add line to history, exiting...");
      exit(1);
    };

    // parsing
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      //print AST
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
 
    /* Free retrieved input */
    linenoiseFree(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  return 0;
}