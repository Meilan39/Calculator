#ifndef GLOBAL
#define GLOBAL

#include <math.h>
#include "../tree/tree.h"

const long double GLOBAL_E;
const long double GLOBAL_PI;
const long double GLOBAL_THRESHOLD;

int get_numeric(Node* head, long double* value);

int get_compound(Node* head, long double* value);
int get_scientific(Node* head, long double* value);
int get_specific_symbol(Node* head, long double* value);
int get_rational(Node* head, long double* value);

int nonvariable_isinteger(Node* head);

#endif