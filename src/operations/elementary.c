#include "elementary.h"

int elementary_resolve(Node* head) {
    elementary_addition(head);    
}

int elementary_addition(Node* head) {
    long double a = 0, b;
    char* end;
    for(int i = 0; i < head->length; i++) {
        if(head->next[i]->type == nt_symbol) continue;
        // elementary_multiplication(head->next[i]);
        // to double
        errno = 0;
        b = strtold(head->next[i]->next[0]->token->s, &end);
        if(errno == ERANGE) goto E;
        // add
        a += b;
        if(!isfinite(a)) goto E;
    }
    return 0;
E:  return -1;
}
int elementary_multiplication(Node* head);
int elementary_exponentiate(Node* head);
int elementary_parenthesis(Node* head);
double elementary_conversion(Node* head);

// elementary_addition(head);
//     n_compress_suffix(this);
//     n_compress_chain(this);
//     for(int i = 0; i < this->length; i++)
//         n_compress(this->next[i]);  