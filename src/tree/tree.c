#include "tree.h"

void n_simplify(Node* this) {
    /* print parse tree */
    n_print(this, "./src/meta/pt.txt");
    /* compress */
    n_compress(this);
    /* print abstract syntax tree */
    n_print(this, "./src/meta/ast.txt");    
}

void n_compress(Node* this) {
    //n_compress_symbol(this);
    n_compress_suffix(this);
    n_compress_chain(this);
    for(int i = 0; i < this->length; i++)
        n_compress(this->next[i]);  
}

Node* n_free(Node* head) {
    if(!head) return NULL;
    if(head->length) {
        for(int i = 0; i < head->length; i++)
            if(head->next[i]) n_free(head->next[i]);
        if(head->next) free(head->next);
    }
    free(head);
    return NULL;
}

Node* n_reset(Node* head) {
    if(!head) return NULL;
    if(head->length) {
        for(int i = 0; i < head->length; i++)
            n_free(head->next[i]);
        if(head->next) free(head->next);
    }
    head->next = NULL;
    head->length = 0;
    return head; 
}

Node* n_construct(int type, const Token* token) {
    Node* this = malloc(sizeof(Node));
    if(!this) return NULL;
    this->next = NULL;
    this->length = 0;
    this->type = type;
    if(token) this->value = token->value;
    return this;
}

int n_push(Node* this, Node* node) {
    if(!node) return 0;
    if((this->length)++) this->next = realloc(this->next, this->length * sizeof(Node*));
    else this->next = malloc(this->length * sizeof(Node*));
    this->next[this->length - 1] = node;   
    return 1;
}

void n_print(Node* this, const char* path) {
    s_abstract_syntax_tree = fopen(path, "w");
    if(!s_abstract_syntax_tree) {printf("unable to open %s\n", path); return;}
    int state[100] = {0};
    n_helper(this, 0, 0, state);
    fclose(s_abstract_syntax_tree);
} 

void n_helper(Node* this, int depth, int edge, int state[]) {
    if(depth > 0) {
        for(int i = 1; i < depth; i++)
            fprintf(s_abstract_syntax_tree, state[i] ? "│   " : "    ");
        fprintf(s_abstract_syntax_tree, "%s", (edge ? "└── " : "├── "));
    }
    if(c_types(this->type)) {
        fprintf(s_abstract_syntax_tree, "%s {%Lf}\n", n_typtostr(this->type), this->value);
    } else fprintf(s_abstract_syntax_tree, "%s\n", n_typtostr(this->type));
    if(this->length > 0) {
        state[depth] = !edge;
        for (int i = 0; i < this->length; i++) {
            n_helper(this->next[i], depth + 1, i == (this->length - 1), state);
        }
        state[depth] = 0;
    }
}

void n_compress_chain(Node* this) {
    if(this->length==0) return;
    for(int i = 0; i < this->length; i++) {
        Node* next = this->next[i];
        while(1) {
            if(n_chain_exception(next->type)) break;
            if(next->length!=1) break;
            this->next[i] = next->next[0]; // i
            free(next->next);
            free(next);                 
        }
    }
}

void n_compress_suffix(Node* this) {
    if(this->length==0) return;
    if( ! n_suffix_exception(this->type)) return;
    for(int i = 0; i < this->length; i++) {
        Node* next = this->next[i];
        if(this->type + 1 != next->type) continue;
        this->length += next->length - 1;
        Node** temp = malloc(this->length * sizeof(Node*));
        int j = 0, k = 0, l = 0;
        for(; j < this->length; j++) {
            if(j < i || j >= i + next->length) {
                temp[j] = this->next[k++];
                continue;
            } else temp[j] = next->next[l++];
            if(j==i) k++;
        }
        free(this->next);
        this->next = temp;
        free(next->next);
        free(next);
    }
}

const char* n_typtostr(int type) {
    if(type < 1 || nt_terminator <= type) return n_typtostr_map[0];
    return n_typtostr_map[type] ? n_typtostr_map[type] : n_typtostr_map[0];
}
int n_suffix_exception(int type) {
    if(type < 1 || nt_terminator <= type) return 0;
    return n_suffix_exception_map[type];
}
int n_chain_exception(int type) {
    if(type < 1 || nt_terminator <= type) return 0;
    return n_chain_exception_map[type];
}

const char* const n_typtostr_map[nt_terminator] = {
    [0] = "",
    /* constants */
    [ct_number] = "number",
    [ct_decimal] = "decimal",
    [ct_zero] = "zero",
    /* lex */
    [lt_plus] = "plus",
    [lt_minus] = "minus",
    [lt_dot] = "dot", 
    [lt_slash] = "slash",
    [lt_caret] = "caret",
    [lt_equal] = "equal",
    [lt_scientific] = "scientific",
    [lt_h_parenthesis] = "head parenthesis",
    [lt_t_parenthesis] = "tail parenthesis",
    [lt_h_bracket] = "head bracket",
    [lt_t_bracket] = "tail bracket",
    [lt_e] = "e",
    [lt_pi] = "pi",
    [lt_x] = "x",
    [lt_zeros] = "zeros",
    [lt_root] = "root",   
    [lt_sqrt] = "square root",  
    [lt_log] = "logarithm",   
    [lt_ln] = "natural logarithm",  
    [lt_sin] = "sin",  
    [lt_cos] = "cos",   
    [lt_tan] = "tan",  
    [lt_asin] = "arcsin",  
    [lt_acos] = "arccos",   
    [lt_atan] = "arctan",  
    [lt_sinh] = "hyperbolic sin",  
    [lt_cosh] = "hyperbolic cos",   
    [lt_tanh] = "hyperbolic tan",  
    /* syn */
    [nt_command] = "command",
    [nt_nonvariable] = "non variable",
    [nt_zeros] = "zeros",
    [nt_polynomial] = "polynomial",
    [nt_polynomial_suffix] = "polynomial suffix",
    [nt_polynomial_term] = "polynomial term",
    [nt_expression] = "expression",    
    [nt_additive_expression] = "additive expression",
    [nt_additive_expression_suffix] = "additive expression suffix",
    [nt_multiplicative_expression] = "multiplicative expression",
    [nt_multiplicative_expression_suffix] = "multiplicative expression suffix",
    [nt_exponential_expression] = "exponential expression",
    [nt_exponential_expression_suffix] = "exponential expression suffix",
    [nt_parenthetical_expression] = "parenthetical expression",
    [nt_functions] = "functions",   
    [nt_root] = "root",   
    [nt_sqrt] = "square root",  
    [nt_log] = "logarithm",   
    [nt_ln] = "natural logarithm",  
    [nt_sin] = "sine",  
    [nt_cos] = "cosine",   
    [nt_tan] = "tangent",  
    [nt_asin] = "arcsin",  
    [nt_acos] = "arccos",   
    [nt_atan] = "arctan",  
    [nt_sinh] = "hyperbolic sin",  
    [nt_cosh] = "hyperbolic cos",   
    [nt_tanh] = "hyperbolic tan",  
    [nt_compound_number] = "compound number",   
    [nt_special_symbols] = "special symbols",
    [nt_real_number] = "real number",
    [nt_scientific] = "scientific",
    [nt_natural] = "natural",
    [nt_integer] = "integer",
    [nt_rational] = "rational",
    [nt_sign] = "sign",
    [nt_variable] = "variable",
    [nt_x] = "x",
};

const char n_chain_exception_map[] = {
    [nt_additive_expression] = 1,
    [nt_multiplicative_expression] = 1,
    [nt_exponential_expression] = 1,
    [nt_parenthetical_expression] = 1,
    [nt_polynomial_term] = 1,
    [nt_nonvariable] = 1,    
    [nt_zeros] = 1,    
    [nt_rational] = 1,
    [nt_special_symbols] = 1,
};

const char n_suffix_exception_map[] = {
    [nt_additive_expression] = 1,
    [nt_multiplicative_expression] = 1,
    [nt_exponential_expression] = 1,
    [nt_polynomial] = 1,
};
