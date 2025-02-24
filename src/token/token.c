#include "token.h"

Token* t_construct() {
    Token* head = malloc(sizeof(Token));
    head->s = NULL;
    head->next = NULL;
    head->type = 0;
    return head;
}

Token* t_destruct(Token* head) {
    while(head) {
        if(head->s) free(head->s);
        Token* curr = head->next;
        free(head);
        head = curr;
    } return NULL;
}

int t_push(Token* head, char* s, int type) {
    if(! (head)) goto E;
    for(; head->next; head = head->next);
    if(! (head->next = t_construct())) goto E;
    if(! (head->next->s = strdup(s))) goto E;
    head->next->type = type;
    return 0;
 E: printf("push allocation failed\n"); return -1;
}

int t_next(Token** current) {
    if((*current)->next) {
        *current = (*current)->next;
        return 0;
    } return -1;
}

void t_print(Token* head) {
    if(head) head = head->next;
    for(; head; head = head->next) {printf("%d: %s\n", head->type, head->s);}
}