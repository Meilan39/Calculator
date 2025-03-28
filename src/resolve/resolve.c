#include "resolve.h"

void resolve(Node* head) {
    int err;
    if((err = simplify_resolve(head))) goto E;
    //if((err = nonvariable_resolve(head))) goto E;
E:  return;
}