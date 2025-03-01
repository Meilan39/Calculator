#include "elementary.h"

int elementary_resolve(Node* head) {
    elementary_addition(head);    
}

int elementary_addition(Node* head) {
    for(int i = 0; i < head->length; i++) {
        if(head->next[i]->type == nt_symbol) continue;
        elementary_multiplication(head->next[i]);
    }
    
}
int elementary_multiplication(Node* head);
int elementary_exponentiate(Node* head);
int elementary_parenthesis(Node* head);

// elementary_addition(head);
//     n_compress_suffix(this);
//     n_compress_chain(this);
//     for(int i = 0; i < this->length; i++)
//         n_compress(this->next[i]);  