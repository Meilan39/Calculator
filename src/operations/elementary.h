#ifndef ELEMENTARY
#define ELEMENTARY

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../tree/tree.h"

int elementary_resolve(Node* head);
int elementary_addition(Node* head);
int elementary_multiplication(Node* head);
int elementary_exponentiate(Node* head);
int elementary_parenthesis(Node* head);
double elementary_conversion(Node* head);

#endif