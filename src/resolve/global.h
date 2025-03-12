#ifndef GLOBAL
#define GLOBAL

#include <math.h>
#include "../tree/tree.h"

const long double GLOBAL_E;
const long double GLOBAL_PI;

int get_numeric(Node* head, long double* value);
long double set_numeric(Node* head);

int numerical_exception(Node* head);
extern const char numerical_exception_map[nt_terminator];

#endif 