#include "syn.h"

int s_syn(Node** head, Token* token) {
    /* open depth map */
    s_syntax_depth_map = fopen("./src/meta/depthmap.txt", "w");
    if(!s_syntax_depth_map) { printf("depthmap failed to open\n"); return -1; }
    /* parse */
    s_F_end_of_token = 0;
    *head = s_command(&token, 0);
    /* close depth map */
    fclose(s_syntax_depth_map);
    /* print abstract syntax tree */
    return *head ? 0 : -1;
}

Node* s_compare(Token** token, int type) {
    if(s_F_end_of_token) return NULL;
    if( (*token)->type == type ) {
        Node* node = n_construct(nt_symbol, *token);
        s_F_end_of_token = t_next(token);
        return node;
    } return NULL;
}

Node* s_command(Token** token, int depth) {
    PRINTMAP(depth, "command", (*token)->s)
    Node* node = n_construct(nt_command, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_elementary(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_root(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_elementary(Token** token, int depth) {
    PRINTMAP(depth, "elementary", (*token)->s)
    Node* node = n_construct(nt_elementary, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_root(Token** token, int depth) {
    PRINTMAP(depth, "root", (*token)->s)
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
    PRINTMAP(depth, "expression", (*token)->s)
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
    PRINTMAP(depth, "variable expression", (*token)->s)
    Node* node = n_construct(nt_variable_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_polynomial(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_nonvariable_expression(Token** token, int depth) {
    PRINTMAP(depth, "nonvariable expression", (*token)->s)
    Node* node = n_construct(nt_variable_expression, NULL);
    Token* ptoken = *token;
    while(n_push(node, s_nonvariable_expression_suffix(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;    
}
Node* s_nonvariable_expression_suffix(Token** token, int depth) {
    PRINTMAP(depth, "nonvariable expression suffix", (*token)->s)
    Node* node = n_construct(nt_nonvariable_expression, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_real_number(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_plus))) goto c3;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto c4;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_dot))) goto c5;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c5;
    goto t;
c5: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_slash))) goto c6;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c6;
    goto t;
c6: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_caret))) goto c7;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c7;
    goto t;
c7: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_parenthesis))) goto c8;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto c8;
    if(!n_push(node, s_compare(token, lt_t_parenthesis))) goto c8;
    goto t;
c8: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_h_bracket))) goto f;
    if(!n_push(node, s_nonvariable_expression(token, depth+1))) goto f;
    if(!n_push(node, s_compare(token, lt_t_bracket))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_polynomial(Token** token, int depth) {
    PRINTMAP(depth, "polynomial", (*token)->s)
    Node* node = n_construct(nt_polynomial, NULL);
    Token* ptoken = *token;
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_variable))) goto c2;
    if(!n_push(node, s_compare(token, lt_caret))) goto c2;
    if(!n_push(node, s_natural(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    n_push(node, s_real_number(token, depth+1));
    if(!n_push(node, s_compare(token, lt_variable))) goto c3;
    goto t;
c3: *token = ptoken; n_reset(node);
    if(!n_push(node, s_real_number(token, depth+1))) goto c4;
    goto t;
c4: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_plus))) goto c5;
    if(!n_push(node, s_polynomial(token, depth+1))) goto c5;
    goto t;
c5: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_minus))) goto f;
    if(!n_push(node, s_polynomial(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_real_number(Token** token, int depth) {
    PRINTMAP(depth, "real number", (*token)->s)
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
    PRINTMAP(depth, "scientific", (*token)->s)
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
    PRINTMAP(depth, "special symbols", (*token)->s)
    Node* node = n_construct(nt_special_symbols, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_special_symbols))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_natural(Token** token, int depth) {
    PRINTMAP(depth, "natural", (*token)->s)
    Node* node = n_construct(nt_natural, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_nonzero))) goto f;
    while(n_push(node, s_digit(token, depth+1)));
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_integer(Token** token, int depth) {
    PRINTMAP(depth, "integer", (*token)->s)
    Node* node = n_construct(nt_integer, NULL);
    Token* ptoken = *token;
    n_push(node, s_sign(token, depth+1));
    if(!n_push(node, s_natural(token, depth+1))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_zero))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_rational(Token** token, int depth) {
    PRINTMAP(depth, "rational", (*token)->s)
    Node* node = n_construct(nt_rational, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_integer(token, depth+1))) goto c2;
    if(!n_push(node, s_compare(token, lt_period))) goto c2;
    while(n_push(node, s_digit(token, depth+1)));
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_integer(token, depth+1))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node; 
}
Node* s_digit(Token** token, int depth) {
    PRINTMAP(depth, "digit", (*token)->s)
    Node* node = n_construct(nt_digit, NULL);
    Token* ptoken = *token;
    if(!n_push(node, s_compare(token, lt_zero))) goto c2;
    goto t;
c2: *token = ptoken; n_reset(node);
    if(!n_push(node, s_compare(token, lt_nonzero))) goto f;
    goto t;
f : *token = ptoken;
    return n_free(node);
t : return node;
}
Node* s_sign(Token** token, int depth) {
    PRINTMAP(depth, "sign", (*token)->s)
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