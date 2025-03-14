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
        Node* node = n_construct(type, *token);
        t_next(token);
        return node;
    } return NULL;
}

Node* s_command(Token** token, int depth) {
    PRINTMAP(depth, "command", token)
    Node* node = n_construct(nt_command, NULL);
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
    Node* node = n_construct(nt_nonvariable, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_zeros(Token** token, int depth) {
    PRINTMAP(depth, "zeros", token)
    Node* node = n_construct(nt_zeros, NULL);
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
    Node* node = n_construct(nt_polynomial, NULL);
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
    Node* node = n_construct(nt_polynomial_suffix, NULL);
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
    Node* node = n_construct(nt_polynomial_term, NULL);
    Token* ptoken = *token;
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_x))) goto c2;
    if(!n_push(node, s_compare(token, lt_caret))) goto c2;
    if(!n_push(node, s_natural(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_x))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_real_number(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_expression(Token** token, int depth) {
    PRINTMAP(depth, "expression", token)
    Node* node = n_construct(nt_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_additive_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}
Node* s_additive_expression(Token** token, int depth) {
    PRINTMAP(depth, "additive expression", token)
    Node* node = n_construct(nt_additive_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto f;
    while(n_push(node, s_additive_expression_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_additive_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "additive expression suffix", token)
    Node* node = n_construct(nt_additive_expression_suffix, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_plus))) goto c2;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto f;
    if(!n_push(node, s_multiplicative_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_multiplicative_expression(Token** token, int depth) {
    PRINTMAP(depth, "multiplicative expression", token)
    Node* node = n_construct(nt_multiplicative_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto f;
    while(n_push(node, s_multiplicative_expression_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_multiplicative_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "multiplicative expression suffix", token)
    Node* node = n_construct(nt_multiplicative_expression_suffix, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_dot))) goto c2;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_slash))) goto f;
    if(!n_push(node, s_exponential_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_exponential_expression(Token** token, int depth) {
    PRINTMAP(depth, "exponential expression", token)
    Node* node = n_construct(nt_exponential_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_parenthetical_expression(token, depth+1))) goto f;
    while(n_push(node, s_exponential_expression_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_exponential_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "exponential expression suffix", token)
    Node* node = n_construct(nt_exponential_expression_suffix, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_caret))) goto f;
    if(!n_push(node, s_parenthetical_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_parenthetical_expression(Token** token, int depth) {
    PRINTMAP(depth, "parenthetical expression", token)
    Node* node = n_construct(nt_parenthetical_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_functions(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compound_number(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_real_number(token, depth+1))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_variable(token, depth+1))) goto c5;
    goto t;
c5: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto c6;
    if(!n_push(node, s_expression(token, depth+1))) goto c6;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto c6;
    goto t;
c6: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_bracket))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_bracket))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_functions(Token** token, int depth) {
    PRINTMAP(depth, "functions", token)
    Node* node = n_construct(nt_functions, NULL);
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
    Node* node = n_construct(nt_root, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_root))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_natural(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sqrt(Token** token, int depth) {
    PRINTMAP(depth, "square root", token)
    Node* node = n_construct(nt_sqrt, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sqrt))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_log(Token** token, int depth) {
    PRINTMAP(depth, "log", token)
    Node* node = n_construct(nt_log, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_log))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_natural(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_ln(Token** token, int depth) {
    PRINTMAP(depth, "natural log", token)
    Node* node = n_construct(nt_ln, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_ln))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sin(Token** token, int depth) {
    PRINTMAP(depth, "sin", token)
    Node* node = n_construct(nt_sin, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sin))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_cos(Token** token, int depth) {
    PRINTMAP(depth, "cos", token)
    Node* node = n_construct(nt_cos, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_cos))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_tan(Token** token, int depth) {
    PRINTMAP(depth, "tan", token)
    Node* node = n_construct(nt_tan, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_tan))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_asin(Token** token, int depth) {
    PRINTMAP(depth, "asin", token)
    Node* node = n_construct(nt_asin, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_asin))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_acos(Token** token, int depth) {
    PRINTMAP(depth, "acos", token)
    Node* node = n_construct(nt_acos, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_acos))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_atan(Token** token, int depth) {
    PRINTMAP(depth, "atan", token)
    Node* node = n_construct(nt_atan, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_atan))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sinh(Token** token, int depth) {
    PRINTMAP(depth, "sin", token)
    Node* node = n_construct(nt_sinh, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_sinh))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_cosh(Token** token, int depth) {
    PRINTMAP(depth, "cosh", token)
    Node* node = n_construct(nt_cosh, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_cosh))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_tanh(Token** token, int depth) {
    PRINTMAP(depth, "tanh", token)
    Node* node = n_construct(nt_tanh, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_tanh))) goto f;
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto f;
    if(!n_push(node, s_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_compound_number(Token** token, int depth) {
    PRINTMAP(depth, "compound number", token)
    Node* node = n_construct(nt_compound_number, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_real_number(token, depth+1))) goto c2;
    if(!n_push(node, s_variable(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_real_number(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_variable(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_real_number(Token** token, int depth) {
    PRINTMAP(depth, "real number", token)
    Node* node = n_construct(nt_real_number, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_irrational(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_scientific(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_rational(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_irrational(Token** token, int depth) {
    PRINTMAP(depth, "irrational", token)
    Node* node = n_construct(nt_irrational, NULL);
    Token* ptoken = *token;
    n_push(node, s_scientific(token, depth+1));
    if(!n_push(node, s_special_symbols(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_rational(token, depth+1));
    if(!n_push(node, s_special_symbols(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_special_symbols(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_scientific(Token** token, int depth) {
    PRINTMAP(depth, "scientific", token)
    Node* node = n_construct(nt_scientific, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_rational(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_scientific))) goto f;
    if(!n_push(node, s_integer(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_natural(Token** token, int depth) {
    PRINTMAP(depth, "natural", token)
    Node* node = n_construct(nt_natural, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, ct_number))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_integer(Token** token, int depth) {
    PRINTMAP(depth, "integer", token)
    Node* node = n_construct(nt_integer, NULL);
    Token* ptoken = *token;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, ct_number))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_zero))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_rational(Token** token, int depth) {
    PRINTMAP(depth, "rational", token)
    Node* node = n_construct(nt_rational, NULL);
    Token* ptoken = *token;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, ct_decimal))) goto c2;    
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, ct_number))) goto c3;    
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, ct_zero))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_special_symbols(Token** token, int depth) {
    PRINTMAP(depth, "special symbols", token)
    Node* node = n_construct(nt_special_symbols, NULL);
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
Node* s_sign(Token** token, int depth) {
    PRINTMAP(depth, "sign", token)
    Node* node = n_construct(nt_sign, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_plus))) goto c2;    
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto f;    
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;   
}
Node* s_variable(Token** token, int depth) {
    PRINTMAP(depth, "variable", token)
    Node* node = n_construct(nt_variable, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_x(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;   
}
Node* s_x(Token** token, int depth) {
    PRINTMAP(depth, "x", token)
    Node* node = n_construct(nt_x, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_x))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}