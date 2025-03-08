#include "nonvariable.h"

int nonvariable_resolve(Node* head) {
    if(head->next[0]->type != nt_nonvariable) return 0;
    NONVARIABLE_SCIENTIFIC = 0;
    long double value;
    if(nonvariable_addition(head->next[0]->next[0], &value) == -1) goto E;
    if(NONVARIABLE_SCIENTIFIC) {
        int i = 0;
        for(; value > 10; i++) { value /= 10; }
        for(; 0 < value && value < 1 ; i--) { value *= 10; }
        printf(" > %Lf x 10^%d\n", value, i);
    } else {
        printf(" > %Lf\n", value);
    }
    return 1;
E:  printf("...double overflow\n");
    return -1;
}

int nonvariable_addition(Node* head, long double* value) {
    long double a = 0, b;
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
int nonvariable_multiplication(Node* head, long double* value) {
    long double a = 1, b;
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
int nonvariable_exponentiation(Node* head, long double* value) {
    long double a, b = 1;
    /* from right to left */
    for(int i = head->length - 1; i >= 0; i--) {
        Node* next = head->next[i];
        if(next->type == lt_caret) {continue;}
        else {
            if(nonvariable_parenthesis(next, &a) == -1) goto E;
            b = powl(a, b);
            if(!isfinite(b)) goto E;
        }
    }
    *value = b;
    return 0;
E:  return -1;
}
int nonvariable_parenthesis(Node* head, long double* value) {
    if(head->length == 1) {
        if(nonvariable_conversion(head->next[0], value) == -1) goto E;
    } else {
        if(nonvariable_addition(head->next[1], value) == -1) goto E;
    }
    return 0;
E:  return -1;
}
int nonvariable_conversion(Node* head, long double* value) {
    long double a;
    int hasSign;
    char* end;
    errno = 0;
    /* convert */
    if(head->type == nt_special_symbols) {
        hasSign = head->length - 1;
        if(head->next[hasSign]->type == lt_e) a = GLOBAL_E;
        if(head->next[hasSign]->type == lt_pi) a = GLOBAL_PI;
        if(head->next[0]->type == lt_minus) a = -a;
    }
    if(head->type == nt_scientific) {
        NONVARIABLE_SCIENTIFIC = 1;
        Node* rational = head->next[0], *exponent = head->next[2];
        /* coefficient part */
        hasSign = rational->length - 1;
        a = strtold(rational->next[hasSign]->token->s, &end);
        if(rational->next[0]->type == lt_minus) a = -a;
        /* exponent part */
        int temp;
        hasSign = exponent->length - 1;
        temp = strtold(exponent->next[hasSign]->token->s, &end);
        if(exponent->next[0]->type == lt_minus) temp = -temp;        
        a *= powl(10, temp);
    }
    if(head->type == nt_rational) {
        hasSign = head->length - 1;
        a = strtold(head->next[hasSign]->token->s, &end);
        if(head->next[0]->type == lt_minus) a = -a;       
    }
    *value = a;
    /* range */
    if(errno == ERANGE || !isfinite(a)) return -1;
    else return 0;
}