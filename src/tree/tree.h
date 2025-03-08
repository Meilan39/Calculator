#ifndef TREE
#define TREE

#include <stdio.h>
#include "../token/token.h"
#include "../lex/lex.h"

FILE* s_abstract_syntax_tree;

typedef struct NODE{
   struct NODE** next;
   int length;
   int type;
   long double value;
} Node;

const char* n_get(int type);

Node* n_free(Node* head);
Node* n_reset(Node* head);
Node* n_construct(int type, const Token* token);
int n_push(Node* this, Node* node);
void n_print(Node* this, const char* path);
void n_helper(Node* this, int depth, int endge, int state[]);
/* simplify */
void n_simplify(Node* this);
void n_compress(Node* this);
void n_compress_symbol(Node* this);
void n_compress_suffix(Node* this);
void n_compress_chain(Node* this);
/* helpers */
int n_symbol_exception(int type);
int n_suffix_exception(int type);
int n_chain_exception(int type);

enum n_Types {
   nt_command = 100,
   nt_nonvariable,
   nt_root,
   nt_expression,
   nt_variable_expression,
   nt_nonvariable_expression,
   nt_polynomial,
   nt_polynomial_term,
   nt_additive_expression,
   nt_additive_expression_suffix,
   nt_multiplicative_expression,
   nt_multiplicative_expression_suffix,
   nt_exponential_expression,
   nt_exponential_expression_suffix,
   nt_parenthetical_expression,
   nt_real_number,
   nt_scientific,
   nt_natural,
   nt_integer,
   nt_rational,
   nt_sign,
   nt_variable,
   nt_special_symbols
};

#endif
