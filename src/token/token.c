#include "token.h"

Token* t_construct() {
    Token* head = malloc(sizeof(Token));
    head->next = NULL;
    head->type = 0;
    head->value = 0;
    return head;
}

Token* t_destruct(Token* head) {
    while(head) {
        Token* curr = head->next;
        free(head);
        head = curr;
    } return NULL;
}

int t_push(Token* head, int type, double value) {
    if(! (head)) goto E;
    for(; head->next; head = head->next);
    if(! (head->next = t_construct())) goto E;
    head->next->type = type;
    head->next->value = value;
    return 0;
 E: printf("token push allocation failed\n"); return -1;
}

int t_next(Token** current) {
    *current = (*current)->next;
    return *current ? 0 : -1;
}

void t_print(Token* head) {
    if(head) head = head->next;
    for(; head; head = head->next) {
        printf("%d: %Lf\n", head->type, head->value);
    }
}