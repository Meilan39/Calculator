#ifndef SIMPLIFY
#define SIMPLIFY

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../tree/tree.h"
#include "./global.h"

Node* SIMPLIFY_ADDITIVE;
Node* SIMPLIFY_MULTIPLICATIVE;
Node* SIMPLIFY_EXPONENTIAL;

int simplify_resolve(Node* head);

int simplify_addition(Node* head);
int simplify_multiplication(Node* head);
int simplify_exponentiation(Node* head);
int simplify_parenthesis(Node* head);

int simplify_additive_compare(Node* a, Node* b);
int simplify_multiplicative_compare(Node* a, Node* b);


#endif