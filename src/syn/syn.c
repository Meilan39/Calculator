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
    if(!n_push(node, s_root(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_nonvariable(Token** token, int depth) {
    PRINTMAP(depth, "non variable", token)
    Node* node = n_construct(nt_nonvariable, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_root(Token** token, int depth) {
    PRINTMAP(depth, "root", token)
    Node* node = n_construct(nt_root, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_root))) goto c2;
    if(!n_push(node, s_polynomial(token, depth+1))) goto c2;
    if(!n_push(node, s_compare(token, lt_equal))) goto c2;
    if(!n_push(node, s_polynomial(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_root))) goto f;
    if(!n_push(node, s_polynomial(token, depth+1))) goto f;   
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_expression(Token** token, int depth) {
    PRINTMAP(depth, "expression", token)
    Node* node = n_construct(nt_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_variable_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_variable_expression(Token** token, int depth) {
    PRINTMAP(depth, "variable expression", token)
    Node* node = n_construct(nt_variable_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_polynomial(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_nonvariable_expression(Token** token, int depth) {
    PRINTMAP(depth, "nonvariable expression", token)
    Node* node = n_construct(nt_nonvariable_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_additive_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}
Node* s_polynomial(Token** token, int depth) {
    PRINTMAP(depth, "polynomial", token)
    Node* node = n_construct(nt_polynomial, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_plus))) goto c2;
    if(!n_push(node, s_polynomial_term(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto c3;
    if(!n_push(node, s_polynomial_term(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
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
    if(!n_push(node, s_real_number(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto c3;
    if(!n_push(node, s_additive_expression(token, depth+1))) goto c3;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_bracket))) goto f;
    if(!n_push(node, s_additive_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_bracket))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_real_number(Token** token, int depth) {
    PRINTMAP(depth, "real number", token)
    Node* node = n_construct(nt_real_number, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_scientific(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_rational(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
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
Node* s_special_symbols(Token** token, int depth) {
    PRINTMAP(depth, "special symbols", token)
    Node* node = n_construct(nt_special_symbols, NULL);
    Token* ptoken = *token;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, lt_e))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_compare(token, lt_pi))) goto f;
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