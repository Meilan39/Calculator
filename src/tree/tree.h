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
/* exceptions */
const char* n_typtostr(int type);
int n_suffix_exception(int type);
int n_chain_exception(int type);

enum n_Types {
   nt_command = lt_terminator + 1,
   nt_nonvariable,
   nt_zeros,
   nt_polynomial,
   nt_polynomial_suffix,
   nt_polynomial_term,
   nt_expression,   
   nt_additive_expression,
   nt_additive_expression_suffix,
   nt_multiplicative_expression,
   nt_multiplicative_expression_suffix,
   nt_exponential_expression,
   nt_exponential_expression_suffix,
   nt_parenthetical_expression,
   nt_functions,
   nt_root,
   nt_sqrt,
   nt_log,
   nt_ln,
   nt_sin,
   nt_cos,
   nt_tan,
   nt_asin, 
   nt_acos,
   nt_atan,
   nt_sinh,
   nt_cosh,
   nt_tanh,
   nt_compound_number,
   nt_real_number,
   nt_scientific,
   nt_special_symbols,
   nt_natural,
   nt_integer,
   nt_rational,
   nt_sign,
   nt_variable,
   nt_x,
   nt_terminator
};

/* exception map */
extern const char* const n_typtostr_map[nt_terminator];
extern const char n_suffix_exception_map[nt_terminator];
extern const char n_chain_exception_map[nt_terminator];

#endif
