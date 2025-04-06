#ifndef SYNTAX
#define SYNTAX

#include <stdio.h>
#include "../token/token.h"
#include "../lex/lex.h"
#include "../tree/tree.h"

FILE* s_syntax_depth_map;

int s_minus_flag;
int s_divide_flag;

#define DEPTHMAP 1
#ifdef DEPTHMAP
    #define PRINTMAP(depth, name, token)                                        \
        if(depth>0) {                                                           \
            for (int i = 1; i < depth; i++)                                     \
                fprintf(s_syntax_depth_map, "│   ");                            \
            fprintf(s_syntax_depth_map, "├── ");                                \
        }                                                                       \
        fprintf(s_syntax_depth_map, "%s\n", name);        
        // if((*token) && c_types((*token)->type))                              
        //     fprintf(s_syntax_depth_map, "%s {%Lf}\n", name, (*token)->value); 
        // else                                                                  
#else
    #define PRINTMAP(fmt, ...)
#endif

int s_syn(Node** head, Token* token);
Node* s_compare(Token** token, int type);

Node* s_command(Token** token, int depth);
Node* s_nonvariable(Token** token, int depth);
Node* s_zeros(Token** token, int depth);
Node* s_polynomial(Token** token, int depth);
Node* s_polynomial_suffix(Token** token, int depth);
Node* s_polynomial_term(Token** token, int depth);
Node* s_expression(Token** token, int depth);
Node* s_additive_expression(Token** token, int depth);
Node* s_additive_expression_suffix(Token** token, int depth);
Node* s_multiplicative_expression(Token** token, int depth);
Node* s_multiplicative_expression_suffix(Token** token, int depth);
Node* s_exponential_expression(Token** token, int depth);
Node* s_exponential_expression_suffix(Token** token, int depth);
Node* s_primary_expression(Token** token, int depth);
Node* s_parenthesis(Token** token, int depth);
Node* s_functions(Token** token, int depth);
Node* s_root(Token** token, int depth);
Node* s_sqrt(Token** token, int depth);
Node* s_log(Token** token, int depth);
Node* s_ln(Token** token, int depth);
Node* s_sin(Token** token, int depth);
Node* s_cos(Token** token, int depth);
Node* s_tan(Token** token, int depth);
Node* s_asin(Token** token, int depth);
Node* s_acos(Token** token, int depth);
Node* s_atan(Token** token, int depth);
Node* s_sinh(Token** token, int depth);
Node* s_cosh(Token** token, int depth);
Node* s_tanh(Token** token, int depth);
Node* s_real_number(Token** token, int depth);
Node* s_rational(Token** token, int depth);
Node* s_irrational(Token** token, int depth);
Node* s_scientific(Token** token, int depth);
Node* s_sign(Token** token, int depth);
Node* s_variable(Token** token, int depth);
Node* s_x(Token** token, int depth);

#endif