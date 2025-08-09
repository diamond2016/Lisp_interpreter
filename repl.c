#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../linenoise/linenoise.h"
#include "../mpc/mpc.h"

#define HISTORY_LEN   1024
#define EXIT_FAILURE 1

// to manage errors in eval
typedef enum {
  LVAL_NUM,
  LVAL_ERR
} lval_type;

typedef enum {
  LVAL_ERR_ZERO, 
  LERR_BAD_OP, 
  LERR_BAD_NUM
} lval_err1;

typedef struct {
  lval_type type;
  long num;
  lval_err1 err;
} lval;

lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  v.err = 0;
  return v;
}
lval lval_err(lval_err1 x) {
  lval v;
  v.type = LVAL_ERR;
  v.num = 0;
  v.err = x;
  return v;
}

int number_of_nodes(mpc_ast_t *tree) {
  assert(tree->children_num >=0 );
  if (tree->children_num == 0) return 1; //(1=root node)

  int num_nodes = 1;
  for (int i = 0; i < tree->children_num; i++) {
    num_nodes += number_of_nodes(tree->children[i]);
  }
  return num_nodes;
}

long eval_operand(char *op, long x, long y) {
  assert(op != NULL);
  if (strcmp(op, "+") == 0) {
    return x + y;
  } else if (strcmp(op, "-") == 0) {
    return x - y;
  } else if (strcmp(op, "*") == 0) {
    return x * y;
  } else if (strcmp(op, "/") == 0) {
    if (y == 0) {
      fprintf(stderr, "Error: Division by zero\n");
      exit(EXIT_FAILURE);
    }           
    return x / y;
  }   
  fprintf(stderr, "Error: Unknown operator %s\n", op);
  exit(EXIT_FAILURE);
}

// evaluate the expression tree
long eval (mpc_ast_t *tree) {
  assert(tree->children_num >= 0);
  assert(tree->tag != NULL);
  assert(tree->contents != NULL);
  long result = 0;

   // if the tag is lispy recurse
  if (strstr(tree->tag, ">") != 0 && tree->children_num > 0) {
    result = eval(tree->children[1]);
  }

// if the tag is expression, and only a child return value
  if ((strstr(tree->tag, "expr") != 0) && (tree->children_num > 0)) {
      char * op = tree->children[1]->contents;
      long result1 = eval(tree->children[2]);
      long result2 = eval(tree->children[3]);
      result = eval_operand(op, result1, result2);
  }   
  
// if the tag is expression, and only a child return value        
    if ((strstr(tree->tag, "number") != 0) && (tree->children_num == 0)) {
      result = strtol(tree->contents, NULL, 10);
    }

  return result;
} 

int main(void) {

  /* Print Version and Exit Information */
  puts("Lispy Version 0.0.0.0.7");
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
    " number : /-?[0-9]+[.[0-9]*]?/;                  "
    " operator : '+'|'-'|'*'|'/';                     "
    " expr : <number>|'('<operator> <expr> <expr>')'; "
    " lispy : /^/<expr>+/$/;                          ",
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

      int n = number_of_nodes(r.output);
      printf("# nodi AST = %d\n", n);

        //print AST and OUTPUT
      printf("AST:\n");
      mpc_ast_print(r.output);
      long result = eval(r.output);
      printf("Result: %ld\n", result);
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