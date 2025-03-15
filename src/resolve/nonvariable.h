#ifndef NONVARIABLE
#define NONVARIABLE

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../tree/tree.h"
#include "./global.h"

int NONVARIABLE_SCIENTIFIC;
extern const double THRESHOLD;

int nonvariable_resolve(Node* head);
int nonvariable_addition(Node* head, double* value);
int nonvariable_multiplication(Node* head, double* value);
int nonvariable_exponentiation(Node* head, double* value);
int nonvariable_parenthesis(Node* head, double* value);
int nonvariable_function(Node* head, double* value);


#endif