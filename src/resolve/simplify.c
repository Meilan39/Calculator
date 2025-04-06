#include "simplify.h"

Node* SIMPLIFY_ADDITIVE = NULL;
Node* SIMPLIFY_MULTIPLICATIVE = NULL;
Node* SIMPLIFY_EXPONENTIAL = NULL;

int simplify_resolve(Node* head) {
    if(simplify_addition(head->next[0]->next[0]) == -1) goto E;
    n_print(head, "./src/meta/simplified.txt");
    return 1;
E:  printf("...double overflow\n");
    return -1;
}

int simplify_addition(Node* head) {
    SIMPLIFY_ADDITIVE = head;
    int idx = 0;
    Node **ptr, *ref;
    for(int i = 0; i < head->length; i++) {
        switch(simplify_multiplication(head->next[i])) {
            case  1: i--; break; // expansion: i - 1 + 1 to offset
            case -1: goto E; break;
        }
    }
    for(int i = 0; i < head->length; i++) {
        if((ptr = n_findd(head->next[i], nt_primary_expression))) {
            ref = *ptr;
            Node* temp = make("{}", nt_primary_expression,
                                    n_pick(ref, 1));
            n_replace(ptr, temp);
        }
    }
    for(int i = 0; i < head->length; i++) {
        /* combine like terms and canonicalize */
        for(int j = 0; j <= idx; j++) {
            int cmp = simplify_additive_compare(head->next[j], head->next[i]);
            if(cmp == 0) {
                /* merge */
                n_delete(head, i);
                break;
            } else if(cmp > 0) {
                for(int k = i - 1; idx <= k; k--) 
                    swap(&head->next[k], &head->next[k+1]);
                idx++;
                break;
            } else if(j == idx) {
                idx++;
                if(idx != i) {
                    head->next[idx] = head->next[i];
                    head->next[i] = NULL;                    
                }
                break;
            }
        }
    }
    return 0;
E:  return -1;
}
int simplify_multiplication(Node* head) {
    SIMPLIFY_MULTIPLICATIVE = head;
    int idx = 0;
    Node **ptr, *ref;
    for(int i = 0; i < head->length; i++) {
        switch(simplify_exponentiation(head->next[i])) {
            case  1: i--; break; // expansion: i - 1 + 1 to offset
            case -1: goto E; break;
        }
    }
    for(int i = 0; i < head->length; i++) {
        if((ptr = n_findd(head->next[i], nt_primary_expression))) {
            ref = *ptr;
            if(ref->next[1]->type == nt_multiplicative_expression) {
                Node* temp = make("{}", nt_primary_expression,
                                        n_pick(ref, 1));
                n_replace(ptr, temp);
            }
            if(head->length == 2) goto N;
            if(ref->next[1]->type == nt_additive_expression) {
                // if(ref->next[1]->length == 1) goto N;
                Node *parenthesis = n_pop(head, i),
                     *primary = parenthesis->next[1];
                for(int j = 0; j < primary->length; j++) {
                    Node* temp = make("{}{}", nt_multiplicative_expression,
                                              n_copy(head),
                                              n_pick(primary, i));
                    n_push(SIMPLIFY_ADDITIVE, temp);
                    n_free(parenthesis);
                }
                n_delete(SIMPLIFY_ADDITIVE, i);
                goto T;
            }
        }       
    }
    for(int i = 0; i < head->length; i++) {
        /* combine like terms and canonicalize */
        for(int j = 0; j <= idx; j++) {
            int cmp = simplify_multiplicative_compare(head->next[j], head->next[i]);
            if(cmp == 0) {
                /* merge */            
                n_delete(head, i);
                break;
            } else if(cmp > 0) {
                for(int k = i - 1; idx <= k; k--) 
                    swap(&head->next[k], &head->next[k+1]);
                idx++;
                break;
            } else if(j == idx) {
                idx++;
                if(idx != i) {
                    head->next[idx] = head->next[i];
                    head->next[i] = NULL;                    
                }
                break;
            }
        }
    }
N:  return 0;
T:  return 1;
E:  return -1;
}
int simplify_exponentiation(Node* head) {
    SIMPLIFY_EXPONENTIAL = head;
    int idx = 0;
    for(int i = head->length - 1; i >= 0; i--) {
        switch(simplify_parenthesis(head->next[i])) {
            case  1: i--; break; // expansion: i - 1 + 1 to offset
            case -1: goto E; break;
        }
    }
    for(int i = head->length - 1; i >= 0; i--) {
        Node **parenthesis, **temp;
        if((parenthesis = n_findd(head->next[i], nt_primary_expression))) {
            if((temp = n_findd((*parenthesis)->next[1], nt_exponential_expression))) {
                Node* temp = make("{}", nt_primary_expression,
                                        n_pick((*parenthesis), 1));
                n_replace(parenthesis, temp);
            }
            if(head->length == 2) goto N;
            if((temp = n_findd((*parenthesis)->next[1], nt_multiplicative_expression))) {
                

            }
            if((temp = n_findd((*parenthesis)->next[1], nt_multiplicative_expression))) {
                Node *parenthesis = n_pop(head, i),
                     *primary = parenthesis->next[1];
                for(int j = 0; j < primary->length; j++) {
                    Node* temp = make("{}{}", nt_multiplicative_expression,
                                              n_copy(head),
                                              n_pick(primary, i));
                    n_push(SIMPLIFY_ADDITIVE, temp);
                    n_free(parenthesis);
                }
                n_delete(SIMPLIFY_ADDITIVE, i);
                goto T;
            }
        }       
    }
    for(int i = head->length - 1; i >= 0; i--) {
        /* combine like terms and canonicalize */
        for(int j = 0; j <= idx; j++) {
            int cmp = simplify_multiplicative_compare(head->next[j], head->next[i]);
            if(cmp == 0) {
                /* merge */            
                n_delete(head, i);
                break;
            } else if(cmp > 0) {
                for(int k = i - 1; idx <= k; k--) 
                    swap(&head->next[k], &head->next[k+1]);
                idx++;
                break;
            } else if(j == idx) {
                idx++;
                if(idx != i) {
                    head->next[idx] = head->next[i];
                    head->next[i] = NULL;                    
                }
                break;
            }
        }
    }
N:  return 0;
T:  return 1;
E:  return -1;
}
int simplify_parenthesis(Node* head) {

}
int simplify_function(Node* head) {
    double base, expression, a;
    switch(head->type) {
        case nt_root:
            if(get_numeric(head->next[2], &base) == -1) goto E;
            if(simplify_addition(head->next[5]) == -1) goto E;
            a = pow(expression, 1 / base);
            if(!isfinite(a)) {
                printf("...unsupported root base\n");
                return -1;
            }
            break;
        case nt_sqrt:
            if(simplify_addition(head->next[2]) == -1) goto E;
            a = sqrt(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_log:
            if(get_numeric(head->next[2], &base) == -1) goto E;
            if(simplify_addition(head->next[5]) == -1) goto E;
            if(base != 2 && base != 10 && base != GLOBAL_E) {
                printf("...unsupported log base\n");
                return -1;
            }
            if(base == 2)        a = log2(expression);
            if(base == 10)       a = log10(expression);
            if(base == GLOBAL_E) a = log(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_ln:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = log(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_sin:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = sin(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_cos:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = cos(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_tan:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = tan(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_asin:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = asin(expression);
            if(!isfinite(a)) {
                printf("...unsupported arcsin range (-1 ~ 1)");
                return -1;
            }
            break;
        case nt_acos:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = acos(expression);
            if(!isfinite(a)) {
                printf("...unsupported arccosine range (-1 ~ 1)");
                return -1;
            }
            break;
        case nt_atan:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = atan(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_sinh:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = atan(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_cosh:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = atan(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_tanh:
            if(get_numeric(head->next[2], &expression) == -1) goto E;
            a = atan(expression);
            if(!isfinite(a)) goto E;
            break;
        default:
            printf("...invalid input for simplify resolve\n");
            goto E;
    }
    return 0;
E:  printf("...double overflow\n");
    return -1;
}

int simplify_is_equal(Node* a, Node* b) {
    if(!a || !b) return 0;
    if(a->type != b->type) return 0;
    if(a->value != b->value) return 0;
    if(a->length != b->length) return 0;
    for(int i = 0; i < a->length; i++)
        return simplify_is_equal(a->next[i], b->next[i]);
    return 1;
}

int simplify_reduce(Node* a, Node* b) {
    if(a->type != nt_multiplicative_expression || b->type != nt_multiplicative_expression)
        goto E;
    return 0;
E:  return -1;  
}
