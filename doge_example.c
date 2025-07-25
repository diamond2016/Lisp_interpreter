#include <stdio.h>
#include <stdlib.h>
#include "mpc/mpc.h"

int main(int argc, char** argv) {

mpc_parser_t *Expr  = mpc_new("expression");
mpc_parser_t *Prod  = mpc_new("product");
mpc_parser_t *Value = mpc_new("value");
mpc_parser_t *Maths = mpc_new("maths");

mpca_lang(MPCA_LANG_DEFAULT,
  " expression : <product> (('+' | '-') <product>)*; "
  " product    : <value>   (('*' | '/')   <value>)*; "
  " value      : /[0-9]+/ | '(' <expression> ')';    "
  " maths      : /^/ <expression> /$/;               ",
  Expr, Prod, Value, Maths, NULL);

mpc_result_t r;
char *input = "(1 + (3 - 2))";
if (mpc_parse("input", input, Maths, &r)) {
  mpc_ast_print(r.output);
  mpc_ast_delete(r.output);
} else {
  mpc_err_print(r.error);
  mpc_err_delete(r.error);
}

mpc_cleanup(4, Expr, Prod, Value, Maths);
}