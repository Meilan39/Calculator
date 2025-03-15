#include "simplify.h"

// int simplify_resolve(Node* head) {
//     if(head->next[0]->type != nt_nonvariable) return 0;
//     double value;
//     if(nonvariable_addition(head->next[0]->next[0]) == -1) goto E;
//     if(1) {
//         int i = 0;
//         for(; value > 10; i++) { value /= 10; }
//         for(; 0 < value && value < 1 ; i--) { value *= 10; }
//         printf(" > %lf x 10^%d\n", value, i);
//     } else {
//         printf(" > %lf\n", value);
//     }
//     return 1;
// E:  printf("...double overflow\n");
//     return -1;
// }

// int nonvariable_addition(Node** head);
// int nonvariable_multiplication(Node** head);
// int nonvariable_exponentiation(Node** head) {
//     Node* head_d = *head;
//     double a, b;
//     /* from right to left */
//     for(int i = head_d->length - 1; i >= 0; i--) {
//         Node* next = head_d->next[i];
//         if(next->type == lt_caret) {continue;}
//         else {
//             if(nonvariable_parenthesis(&next) == -1) goto E;
//             b = powl(a, b);
//             if(!isfinite(b)) goto E;
//         }
//     }
//     return 0;
// E:  return -1;
// }
// int nonvariable_parenthesis(Node** head) {
//     Node* head_d = *head;
//     if(head_d->length == 1) {
//         n_replace(head, &(head_d->next[0]));
//     } else {
//         if(nonvariable_addition(&(head_d->next[1])) == -1) goto E;
//         n_replace(head, &(head_d->next[1]));
//     }
//     return 0;
// E:  return -1;
// }