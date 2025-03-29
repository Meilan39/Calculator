#include "simplify.h"


int simplify_resolve(Node* head) {
    if(simplify_addition(head->next[0]->next[0]) == -1) goto E;
    n_print(head, "./src/meta/simplified.txt");
    return 1;
E:  printf("...double overflow\n");
    return -1;
}

int simplify_addition(Node* head) {
    for(int i = 0; i < head->length; i++) {
        Node *temp, *ref, *rRational, *tRational;
        double rn = 1, rd = 1, tn = 1, td = 1;
        int minus = 0;
        /* pass-over and mark signs */
        if(head->next[i]->type == lt_plus) {minus = 0; continue; }
        if(head->next[i]->type == lt_minus) {minus = 1; continue; } 
        /* remove parenthesis */
        temp = n_get(head->next[i], nt_primary_expression);
        if(temp->length == 3) {
            n_delete(temp, 2, 0, 2);
        }
        /* get multiplication sign */
        temp = n_get(head->next[i], nt_multiplicative_expression);
        if(temp->next[0]->type == lt_minus || temp->next[0]->type == lt_plus) {
            if(temp->next[0]->type == lt_minus)
                minus = !minus;
            n_delete(temp, 1, 0);
        }
        /* compress */
        temp = n_get(head->next[i], nt_multiplicative_expression);
        for(int j = 0; j < i; j+=2) {
            int matched, rDivide = 0, tDivide = 0;
            /* match against reference terms */
            ref = n_get(head->next[j], nt_multiplicative_expression);
            for(int k = 0; k < ref->length; k+=2) {
                matched = 0;
                if(ref->next[k]->type == lt_dot) {rDivide = 0; continue;}
                if(ref->next[k]->type == lt_slash) {rDivide = 1; continue;} 
                if(n_get(ref->next[k], nt_rational)) {
                    continue;
                }
                for(int l = 0; l < temp->length; l+=2) {
                    if(temp->next[l]->type == lt_dot) {tDivide = 0; continue;}
                    if(temp->next[l]->type == lt_slash) {tDivide = 1; continue;} 
                    if(n_get(temp->next[l], nt_rational)) {
                        continue;                        
                    }
                    if(rDivide == tDivide && simplify_is_equal(ref->next[k], temp->next[l])) {
                        matched = 1;
                        break;
                    }
                }
                if(!matched) {
                    ref = NULL;
                    break;
                }
            }
        }
        /* merge temp into reference */
        if(ref) {
            // need to make temp its own thing that can handle fractions.
            double coefficient = (rRational ? rRational->next[0]->value : 1) +
                                 (tRational ? tRational->next[0]->value : 1) *
                                 (minus ? -1 : 1);
            if(!isfinite(coefficient)) {
                printf("...double overflow in additive simplify\n");
                goto E;
            } else if(coefficient == 0) {
                n_delete(head, 4, abs(j-1), j, abs(i-1), i);
            } else {
                n_delete(head, 2, abs(i-1), i);
                if(rRational) {
                    rRational->next[0]->value = coefficient;
                } else {
                    Node *i_number = n_construct(ct_number, coefficient),
                        *i_rational = n_construct(nt_rational, 0);
                    n_push(i_rational, i_number);
                    n_push(ref, i_rational);
                }
            }
        }
    }
    return 0;
E:  return -1;
}
// int simplify_multiplication(Node** head) {
//     Node* multiplication = n_construct(nt_multiplicative_expression, 0);
//     int divide = 0;
//     for(int i = 0; i < (*head)->length; i++) {
//         Node* next = (*head)->next[i];
//         if(next->type == lt_dot) {divide = 0; continue;}
//         if(next->type == lt_slash) {divide = 1; continue;} 
//         if(simplify_exponentiation(&((*head)->next[i])) == -1) goto E;
//         simplify_merge_multiplication(multiplication, &((*head)->next[i]));
//     }
//     n_free(*head);
//     *head = multiplication;
//     return 0;
// E:  return -1;
// }
// int simplify_exponentiation(Node** head) {
//     Node* exponentiation = n_construct(nt_exponential_expression, 0);
//     /* from right to left */
//     for(int i = (*head)->length - 1; i >= 0; i--) {
//         Node* next = (*head)->next[i];
//         if(next->type == lt_caret) {continue;}
//         if(simplify_parenthesis(&((*head)->next[i])) == -1) goto E;
//         simplify_merge_exponentiation(exponentiation, &((*head)->next[i]));
//     }
//     n_free(*head);
//     *head = exponentiation;
//     return 0;
// E:  return -1;
// }
// int simplify_parenthesis(Node** head) {
//     if((*head)->next[0]->type == lt_h_parenthesis) {
//         if(simplify_addition(&((*head)->next[1])) == -1) goto E;
//         simplify_merge_parenthesis(head);
//     } 
//     if((*head)->next[0]->type == nt_functions) {
//         if(simplify_function((*head)->next[0]) == -1) goto E;
//     }
//     return 0;
// E:  return -1;
// }
// int simplify_function(Node** head) {
//     double base, expression, a;
//     switch(head->type) {
//         case nt_root:
//             if(get_numeric(head->next[2], &base) == -1) goto E;
//             if(simplify_addition(head->next[5]) == -1) goto E;
//             a = pow(expression, 1 / base);
//             if(!isfinite(a)) {
//                 printf("...unsupported root base\n");
//                 return -1;
//             }
//             break;
//         case nt_sqrt:
//             if(simplify_addition(head->next[2]) == -1) goto E;
//             a = sqrt(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_log:
//             if(get_numeric(head->next[2], &base) == -1) goto E;
//             if(simplify_addition(head->next[5]) == -1) goto E;
//             if(base != 2 && base != 10 && base != GLOBAL_E) {
//                 printf("...unsupported log base\n");
//                 return -1;
//             }
//             if(base == 2)        a = log2(expression);
//             if(base == 10)       a = log10(expression);
//             if(base == GLOBAL_E) a = log(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_ln:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = log(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_sin:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = sin(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_cos:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = cos(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_tan:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = tan(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_asin:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = asin(expression);
//             if(!isfinite(a)) {
//                 printf("...unsupported arcsin range (-1 ~ 1)");
//                 return -1;
//             }
//             break;
//         case nt_acos:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = acos(expression);
//             if(!isfinite(a)) {
//                 printf("...unsupported arccosine range (-1 ~ 1)");
//                 return -1;
//             }
//             break;
//         case nt_atan:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = atan(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_sinh:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = atan(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_cosh:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = atan(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         case nt_tanh:
//             if(get_numeric(head->next[2], &expression) == -1) goto E;
//             a = atan(expression);
//             if(!isfinite(a)) goto E;
//             break;
//         default:
//             printf("...invalid input for simplify resolve\n");
//             goto E;
//     }
//     return 0;
// E:  printf("...double overflow\n");
//     return -1;
// }

int simplify_is_equal(Node* a, Node* b) {
    if(!a || !b) return 0;
    if(a->type != b->type) return 0;
    if(a->value != b->value) return 0;
    if(a->length != b->length) return 0;
    for(int i = 0; i < a->length; i++)
        return simplify_is_equal(a->next[i], b->next[i]);
    return 1;
}