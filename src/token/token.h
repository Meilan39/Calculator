#ifndef TOKEN
#define TOKEN

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

typedef struct _Token {
    long double value;
    int type;
    struct _Token* next;
} Token;

Token* t_construct();
Token* t_destruct(Token* head);
int t_push(Token* head, int type, long double value);
int t_next(Token** curent);
void t_print(Token* head);

#endif