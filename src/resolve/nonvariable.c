#include "nonvariable.h"

const double THRESHOLD = 0.00000001;

int nonvariable_resolve(Node* head) {
    if(head->next[0]->type != nt_nonvariable) return 0;
    NONVARIABLE_SCIENTIFIC = 0;
    double value;
    if(nonvariable_addition(head->next[0]->next[0], &value) == -1) goto E;
    if(NONVARIABLE_SCIENTIFIC) {
        int i = 0;
        for(; value > 10; i++) { value /= 10; }
        for(; 0 < value && value < 1 ; i--) { value *= 10; }
        printf(" > %lf x 10^%d\n", value, i);
    } else {
        printf(" > %lf\n", value);
    }
    return 1;
E:  printf("...double overflow\n");
    return -1;
}

int nonvariable_addition(Node* head, double* value) {
    double a = 0, b;
    int minus = 0;
    for(int i = 0; i < head->length; i++) {
        Node* next = head->next[i];
        if(next->type == lt_plus) {minus = 0; continue; }
        if(next->type == lt_minus) {minus = 1; continue; } 
        if(nonvariable_multiplication(next, &b) == -1) goto E;
        a = minus ? a - b : a + b;
        if(!isfinite(a)) goto E;
    }
    *value = a;
    return 0;
E:  return -1;
}
int nonvariable_multiplication(Node* head, double* value) {
    double a = 1, b;
    int divide = 0;
    for(int i = 0; i < head->length; i++) {
        Node* next = head->next[i];
        if(next->type == lt_dot) {divide = 0; continue;}
        if(next->type == lt_slash) {divide = 1; continue;} 
        if(nonvariable_exponentiation(next, &b) == -1) goto E;
        a = divide ? a / b : a * b;
        if(!isfinite(a)) goto E;
    }
    *value = a;
    return 0;
E:  return -1;
}
int nonvariable_exponentiation(Node* head, double* value) {
    double a, b = 1;
    /* from right to left */
    for(int i = head->length - 1; i >= 0; i--) {
        Node* next = head->next[i];
        if(next->type == lt_caret) {continue;}
        else {
            if(nonvariable_parenthesis(next, &a) == -1) goto E;
            b = pow(a, b);
            if(!isfinite(b)) goto E;
        }
    }
    *value = b;
    return 0;
E:  return -1;
}
int nonvariable_parenthesis(Node* head, double* value) {
    if(head->next[0]->type == 1) { // fix fix fix fix
        Node* next = head->next[0];
        double a = 1;
        if(next->length != 1) {
            printf("...unable to evaluate (variable in nonvariable expression)");
            return -1;
        }
        if(next->next[0]->type == nt_irrational) {

            next = next->next[0];
        }
        if(next->next[0]->type == nt_scientific) NONVARIABLE_SCIENTIFIC = 1; // fix fix fix fix
        if(get_numeric(head->next[0], value) == -1) goto E;
        *value = a;
    } else if(head->length == 3) {
        if(nonvariable_addition(head->next[1], value) == -1) goto E;
    } else {
        if(nonvariable_function(head->next[0], value) == -1) goto E;
    }
    return 0;
E:  return -1;
}
int nonvariable_function(Node* head, double* value) {
    double base, expression, a;
    switch(head->type) {
        case nt_root:
            if(get_numeric(head->next[2], &base) == -1) goto E;
            if(nonvariable_addition(head->next[5], &expression) == -1) goto E;
            a = pow(expression, 1 / base);
            if(!isfinite(a)) {
                printf("...unsupported root base\n");
                return -1;
            }
            break;
        case nt_sqrt:
            if(nonvariable_addition(head->next[2], &expression) == -1) goto E;
            a = sqrt(expression);
            if(!isfinite(a)) goto E;
            break;
        case nt_log:
            if(get_numeric(head->next[2], &base) == -1) goto E;
            if(nonvariable_addition(head->next[5], &expression) == -1) goto E;
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
            printf("...invalid input for nonvariable resolve\n");
            goto E;
    }
    return 0;
E:  printf("...double overflow\n");
    return -1;
}

