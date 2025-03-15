#ifndef ROOT
#define ROOT

#include <math.h>
#include "../tree/tree.h"
#include "global.h"

int root_resolve(Node* head);

int root_degree(Node* head, int* degree);
int root_get_degree(Node* head, int* degree);
int root_coefficients(Node* head, double* coefficients, int degree);

int root_second_degree(Node* head);
int root_third_degree(Node* head); 

#endif