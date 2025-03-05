#include "resolve.h"

void resolve(Node* head) {
    int err;
    if((err = nonvariable_resolve(head))) goto E;
E:  return;
}