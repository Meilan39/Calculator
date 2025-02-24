#include "tree.h"

Node* n_free(Node* head) {
    if(!head) return NULL;
    if(head->length) {
        for(int i = 0; i < head->length; i++)
            n_free(head->next[i]);
        free(head->next);
    }
    free(head);
    return NULL;
}

Node* n_reset(Node* head) {
    if(!head) return NULL;
    if(head->length) {
        for(int i = 0; i < head->length; i++)
            n_free(head->next[i]);
        free(head->next);
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
    this->token = token;
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
    if(!s_abstract_syntax_tree) {printf("%s\n", path); return;}
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
    if(this->token) {
        fprintf(s_abstract_syntax_tree, "%s {%s}\n", n_get(this->type), this->token->s);
    } else fprintf(s_abstract_syntax_tree, "%s\n", n_get(this->type));
    if(this->length > 0) {
        state[depth] = !edge;
        for (int i = 0; i < this->length; i++) {
            n_helper(this->next[i], depth + 1, i == (this->length - 1), state);
        }
        state[depth] = 0;
    }
}

void n_simplify(Node* this) {
    /* print parse tree */
    n_print(this, "./src/meta/pt.txt");
    /* compress */
    n_compress_symbol(this);
    n_compress_chain(this);
    for(int i = 0; i < this->length; i++){
        n_simplify(this->next[i]);
    }
    /* print abstract syntax tree */
    n_print(this, "./src/meta/ast.txt");    
}

void n_compress_symbol(Node* this) {
    if(this->length!=1) return;
    Node* next = this->next[0];
    if(next->type!=nt_symbol) return;
    this->token = next->token;
    this->length = 0;
    this->next = NULL;
    free(next);
}

void n_compress_chain(Node* this) {
    if(this->length==0) return;
    for(int i = 0; i < this->length; i++) {
        while(1) {
            Node* next = this->next[i];
            if(next->length!=1) break;
            if(next->next[0]->type==nt_symbol) break;
            this->next[i] = next->next[0];
            free(next->next);
            free(next);
        }
    }
}

// void m_compress_suffix(Node* this) {
//     if(this->length==0) return;
//     switch(this->type) {
//         case nt_command: break;
//         default: return; // no suffix to compress
//     }
//     for(int i = 0; i < this->length; i++) {
//         Node* next = this->next[i];
//         if(this->type + 1 != next->type) continue;
//         this->length += next->length - 1;
//         Node** temp = malloc(this->length * sizeof(Node*));
//         int j = 0, k = 0, l = 0;
//         for(; j < this->length; j++) {
//             if(j < i || j >= i + next->length) {
//                 temp[j] = this->next[k++];
//                 continue;
//             } else temp[j] = next->next[l++];
//             if(j==i) k++;
//         }
//         free(this->next);
//         this->next = temp;
//         free(next->next);
//         free(next);
//     }
// }

const char* n_get(int type) {
    switch (type) {
        case nt_command: return "command";
        case nt_elementary: return "elementary";
        case nt_root: return "root";
        case nt_expression: return "expression";
        case nt_variable_expression: return "variable expression";
        case nt_nonvariable_expression: return "nonvariable expression";
        case nt_nonvariable_expression_suffix: return "nonvariable expression suffix";
        case nt_polynomial: return "polynomial";
        case nt_polynomial_suffix: return "polynomial suffix";
        case nt_real_number: return "real number";
        case nt_scientific: return "scientific";
        case nt_special_symbols: return "special symbols";
        case nt_natural: return "natural";
        case nt_integer: return "integer";
        case nt_rational: return "rational";
        case nt_nonzero: return "nonzero";
        case nt_zero: return "zero";
        case nt_digit: return "digit";
        case nt_sign: return "sign";
        case nt_variable: return "variable";
        case nt_symbol: return "symbol";
        default: return "";
    }
}

