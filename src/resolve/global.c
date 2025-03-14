#include "global.h"

const long double GLOBAL_E = 2.71828182;
const long double GLOBAL_PI = 3.14159265;
const long double GLOBAL_THRESHOLD = 0.00000001;

int get_numeric(Node* head, long double* value) {
    errno = 0;
    /* convert */
    switch(head->type) {
        case nt_compound_number: 
            get_compound(head, value); 
            break;
        case nt_scientific: 
            get_scientific(head, value); 
            break;
        case nt_special_symbols: 
            get_specific_symbol(head, value); 
            break;
        case nt_rational:
        case nt_natural:
        case nt_integer:
            get_rational(head, value);
            break;
        default: 
            printf("...unable to evaluate explicitly");
            return -1;
    }
    /* range */
    if(errno == ERANGE || !isfinite(*value)) {
        printf("...double overflow");
        return -1;
    } else return 0;
}

int get_compound(Node* head, long double* value) {
    int hasSign = head->length - 1;
    if(head->next[hasSign]->type == lt_e) *value = GLOBAL_E;
    if(head->next[hasSign]->type == lt_pi) *value = GLOBAL_PI;
    if(head->next[0]->type == lt_minus) *value = -(*value);
}
int get_scientific(Node* head, long double* value) {
    long double rational, exponent;
    get_rational(head->next[0], &rational);
    get_rational(head->next[2], &exponent);
    *value = rational * powl(10, exponent);
}
int get_specific_symbol(Node* head, long double* value) {
    int hasSign = head->length - 1;
    if(head->next[hasSign]->type == lt_e) *value = GLOBAL_E;
    if(head->next[hasSign]->type == lt_pi) *value = GLOBAL_PI;
    if(head->next[0]->type == lt_minus) *value = -(*value);
}
int get_rational(Node* head, long double* value) {
    int hasSign = head->length - 1;
    *value = head->next[hasSign]->value;
    if(head->next[0]->type == lt_minus) *value = -(*value);          
}

int nonvariable_isinteger(Node* head) {
    long double value;
    get_numeric(head, &value);
    return (value - floorl(value) < GLOBAL_THRESHOLD);
}