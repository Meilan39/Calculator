#ifndef GLOBAL
#define GLOBAL

#include <math.h>
#include "../tree/tree.h"

const double GLOBAL_E;
const double GLOBAL_PI;
const double GLOBAL_THRESHOLD;

int get_numeric(Node* head, double* value);

int get_irrational(Node* head, double* value);
int get_scientific(Node* head, double* value);
int get_specific_symbol(Node* head, double* value);
int get_rational(Node* head, double* value);

int isInteger(Node* head);
int isSign(Node* head);

#endif