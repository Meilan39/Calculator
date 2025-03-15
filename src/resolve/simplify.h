#ifndef SIMPLIFY
#define SIMPLIFY

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../tree/tree.h"
#include "./global.h"

int simplify_resolve(Node* head);

int simplify_consolidate_multiplicative(Node* head);
int simplify_consolidate_additive(Node* head);

int simplify_factor(Node* head);

#endif