#include "syn.h"

int s_syn(Node** head, Token* token) {
    /* open depth map */
    s_syntax_depth_map = fopen("./src/meta/depthmap.txt", "w");
    if(!s_syntax_depth_map) { printf("unable to open depthmap\n"); return -1; }
    /* parse */
    *head = s_command(&token, 0);
    /* close depth map */
    fclose(s_syntax_depth_map);
    /* print abstract syntax tree */
    if( (*head)==NULL ) return -1;
    if( token!=NULL ) return -1;
    return 0;
}

Node* s_compare(Token** token, int type) {
    if( (*token)==NULL ) return NULL;
    if( (*token)->type == type ) {
        Node* node = n_construct(type, (*token)->value);
        t_next(token);
        return node;
    } return NULL;
}

Node* s_command(Token** token, int depth) {
    PRINTMAP(depth, "command", token)
    Node* node = n_construct(nt_command, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_nonvariable(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_zeros(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_nonvariable(Token** token, int depth) {
    PRINTMAP(depth, "nonvariable", token)
    Node* node = n_construct(nt_nonvariable, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_zeros(Token** token, int depth) {
    PRINTMAP(depth, "zeros", token)
    Node* node = n_construct(nt_zeros, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_zeros))) goto f;
    if(!n_push(node, s_polynomial(token, depth+1))) goto f;   
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_polynomial(Token** token, int depth) {
    PRINTMAP(depth, "polynomial", token)
    Node* node = n_construct(nt_polynomial, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_polynomial_term(token, depth+1))) goto f;
    while(n_push(node, s_polynomial_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_polynomial_suffix(Token** token, int depth) {
    PRINTMAP(depth, "polynomial suffix", token)
    Node* node = n_construct(nt_polynomial_suffix, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_plus))) goto c2;
    if(!n_push(node, s_polynomial_term(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto f;
    if(!n_push(node, s_polynomial_term(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_polynomial_term(Token** token, int depth) {
    PRINTMAP(depth, "polynomial term", token)
    Node* node = n_construct(nt_polynomial_term, 0);
    Token* ptoken = *token;
    n_push(node, s_sign(token, depth+1));
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_x))) goto c2;
    if(!n_push(node, s_compare(token, lt_caret))) goto c2;
    if(!n_push(node, s_compare(token, ct_number))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_sign(token, depth+1));
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_x))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);    
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_real_number(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_expression(Token** token, int depth) {
    PRINTMAP(depth, "expression", token)
    Node* node = n_construct(nt_expression, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_additive_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}
Node* s_additive_expression(Token** token, int depth) {
    PRINTMAP(depth, "additive expression", token)
    Node* node = n_construct(nt_additive_expression, 0);
    Token* ptoken = *token;
    s_minus_flag = 0;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto f;
    while(n_push(node, s_additive_expression_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_additive_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "additive expression suffix", token)
    Node* node = n_construct(nt_additive_expression_suffix, 0);
    Token* ptoken = *token;
    if(!s_compare(token, lt_plus)) goto c2; s_minus_flag = 0;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!s_compare(token, lt_minus)) goto f; s_minus_flag = 1;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_multiplicative_expression(Token** token, int depth) {
    PRINTMAP(depth, "multiplicative expression", token)
    Node* node = n_construct(nt_multiplicative_expression, 0);
    Token* ptoken = *token;
    s_divide_flag = 0;
    s_sign(token, depth+1); 
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto f;
    while(n_push(node, s_multiplicative_expression_suffix(token, depth+1)));
    node->subtype = s_minus_flag ? 2 : 1;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_multiplicative_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "multiplicative expression suffix", token)
    Node* node = n_construct(nt_multiplicative_expression_suffix, 0);
    Token* ptoken = *token;
    if(!s_compare(token, lt_dot)) goto c2; s_divide_flag = 0;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!s_compare(token, lt_slash)) goto c3; s_divide_flag = 1;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    s_divide_flag = 0;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_exponential_expression(Token** token, int depth) {
    PRINTMAP(depth, "exponential expression", token)
    Node* node = n_construct(nt_exponential_expression, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    while(n_push(node, s_exponential_expression_suffix(token, depth+1)));
    node->subtype = s_divide_flag ? 2 : 1;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_exponential_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "exponential expression suffix", token)
    Node* node = n_construct(nt_exponential_expression_suffix, 0);
    Token* ptoken = *token;
    if(!s_compare(token, lt_caret)) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_primary_expression(Token** token, int depth) {
    PRINTMAP(depth, "primary expression", token)
    Node* node = n_construct(nt_primary_expression, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_real_number(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_functions(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_variable(token, depth+1))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_parenthesis(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_parenthesis(Token** token, int depth) {
    PRINTMAP(depth, "parenthesis", token)
    Node* node = n_construct(nt_parenthesis, 0);
    Token* ptoken = *token;
    if(!s_compare(token, lt_h_parenthesis)) goto c2;
    if(!n_push(node, s_expression(token, depth+1))) goto c2;
    if(!s_compare(token, lt_t_parenthesis)) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!s_compare(token, lt_h_bracket)) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!s_compare(token, lt_t_bracket)) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_functions(Token** token, int depth) {
    PRINTMAP(depth, "functions", token)
    Node* node = n_construct(nt_functions, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_root(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_sqrt(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_log(token, depth+1))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_ln(token, depth+1))) goto c5;
    goto t;
c5: *token = ptoken; n_reset(node);
    if(!n_push(node, s_sin(token, depth+1))) goto c6;
    goto t;
c6: *token = ptoken; n_reset(node);
    if(!n_push(node, s_cos(token, depth+1))) goto c7;
    goto t;
c7: *token = ptoken; n_reset(node);
    if(!n_push(node, s_tan(token, depth+1))) goto c8;
    goto t;
c8: *token = ptoken; n_reset(node);
    if(!n_push(node, s_asin(token, depth+1))) goto c9;
    goto t;
c9: *token = ptoken; n_reset(node);
    if(!n_push(node, s_acos(token, depth+1))) goto c10;
    goto t;
c10:*token = ptoken; n_reset(node);
    if(!n_push(node, s_atan(token, depth+1))) goto c11;
    goto t;
c11:*token = ptoken; n_reset(node);
    if(!n_push(node, s_sinh(token, depth+1))) goto c12;
    goto t;
c12:*token = ptoken; n_reset(node);
    if(!n_push(node, s_cosh(token, depth+1))) goto c13;
    goto t;
c13: *token = ptoken; n_reset(node);
    if(!n_push(node, s_tanh(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_root(Token** token, int depth) {
    PRINTMAP(depth, "root", token)
    Node* node = n_construct(nt_root, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_root))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_compare(token, ct_number))) goto f;
    if(!n_push(node, s_compare(token, lt_comma))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sqrt(Token** token, int depth) {
    PRINTMAP(depth, "square root", token)
    Node* node = n_construct(nt_sqrt, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sqrt))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_log(Token** token, int depth) {
    PRINTMAP(depth, "log", token)
    Node* node = n_construct(nt_log, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_log))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_compare(token, ct_number))) goto f;
    if(!n_push(node, s_compare(token, lt_comma))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_ln(Token** token, int depth) {
    PRINTMAP(depth, "natural log", token)
    Node* node = n_construct(nt_ln, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_ln))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sin(Token** token, int depth) {
    PRINTMAP(depth, "sin", token)
    Node* node = n_construct(nt_sin, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sin))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_cos(Token** token, int depth) {
    PRINTMAP(depth, "cos", token)
    Node* node = n_construct(nt_cos, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_cos))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_tan(Token** token, int depth) {
    PRINTMAP(depth, "tan", token)
    Node* node = n_construct(nt_tan, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_tan))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_asin(Token** token, int depth) {
    PRINTMAP(depth, "asin", token)
    Node* node = n_construct(nt_asin, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_asin))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_acos(Token** token, int depth) {
    PRINTMAP(depth, "acos", token)
    Node* node = n_construct(nt_acos, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_acos))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_atan(Token** token, int depth) {
    PRINTMAP(depth, "atan", token)
    Node* node = n_construct(nt_atan, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_atan))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sinh(Token** token, int depth) {
    PRINTMAP(depth, "sin", token)
    Node* node = n_construct(nt_sinh, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sinh))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_cosh(Token** token, int depth) {
    PRINTMAP(depth, "cosh", token)
    Node* node = n_construct(nt_cosh, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_cosh))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_tanh(Token** token, int depth) {
    PRINTMAP(depth, "tanh", token)
    Node* node = n_construct(nt_tanh, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_tanh))) goto f;
    if(!n_push(node, s_primary_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_real_number(Token** token, int depth) {
    PRINTMAP(depth, "real number", token)
    Node* node = n_construct(nt_real_number, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_rational(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_irrational(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_rational(Token** token, int depth) {
    PRINTMAP(depth, "rational", token)
    Node* node = n_construct(nt_rational, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_scientific(token, depth+1))) goto c2; 
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_decimal))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_number))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_zero))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_irrational(Token** token, int depth) {
    PRINTMAP(depth, "irrational", token)
    Node* node = n_construct(nt_irrational, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_e))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_pi))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_scientific(Token** token, int depth) {
    PRINTMAP(depth, "scientific", token)
    Node* node = n_construct(nt_scientific, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, ct_decimal))) goto c2;
    if(!n_push(node, s_compare(token, lt_scientific))) goto c2;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, ct_number))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_number))) goto f;
    if(!n_push(node, s_compare(token, lt_scientific))) goto f;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, ct_number))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sign(Token** token, int depth) {
    PRINTMAP(depth, "sign", token)
    Node* node = n_construct(nt_sign, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_plus))) goto c2; 
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto f; 
    s_minus_flag = !s_minus_flag;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;   
}
Node* s_variable(Token** token, int depth) {
    PRINTMAP(depth, "variable", token)
    Node* node = n_construct(nt_variable, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_x(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;   
}
Node* s_x(Token** token, int depth) {
    PRINTMAP(depth, "x", token)
    Node* node = n_construct(nt_x, 0);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_x))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}