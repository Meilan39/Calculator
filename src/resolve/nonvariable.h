#ifndef NONVARIABLE
#define NONVARIABLE

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../tree/tree.h"
#include "./global.h"

int NONVARIABLE_SCIENTIFIC;
extern const long double THRESHOLD;

int nonvariable_resolve(Node* head);
int nonvariable_addition(Node* head, long double* value);
int nonvariable_multiplication(Node* head, long double* value);
int nonvariable_exponentiation(Node* head, long double* value);
int nonvariable_parenthesis(Node* head, long double* value);
// int nonvariable_conversion(Node* head, long double* value);

#endif