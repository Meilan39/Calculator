#include "global.h"

const double GLOBAL_E  = 2.718281828459045;
const double GLOBAL_PI = 3.141592653589793;
const double GLOBAL_THRESHOLD = 0.00000001;

int get_numeric(Node* head, double* value) {
    errno = 0;
    /* convert */
    switch(head->type) {
        case nt_irrational: 
            get_irrational(head, value); 
            break;
        case nt_scientific: 
            get_scientific(head, value); 
            break;
        case nt_rational:
            get_rational(head, value);
            break;
        default: 
            printf("...failed to numerically resolve non-numeric\n");
            return -1;
    }
    /* range */
    if(errno == ERANGE || !isfinite(*value)) {
        printf("...double overflow\n");
        return -1;
    } else return 0;
}

int get_irrational(Node* head, double* value) {
    double rational, irrational;
    if(isSign(head->next[0])) {
        rational = head->next[0]->type == lt_minus ? -1 : 1;
    } else { get_numeric(head->next[0], &rational); }
    get_numeric(head->next[1], &irrational);
    *value = rational * irrational;
}
int get_scientific(Node* head, double* value) {
    double rational, exponent;
    get_rational(head->next[0], &rational);
    get_rational(head->next[2], &exponent);
    *value = rational * powl(10, exponent);
}
int get_specific_symbol(Node* head, double* value) {
    int hasSign = head->length - 1;
    if(head->next[hasSign]->type == lt_e) *value = GLOBAL_E;
    if(head->next[hasSign]->type == lt_pi) *value = GLOBAL_PI;
    if(head->next[0]->type == lt_minus) *value = -(*value);
}
int get_rational(Node* head, double* value) {
    int hasSign = head->length - 1;
    *value = head->next[hasSign]->value;
    if(head->next[0]->type == lt_minus) *value = -(*value);          
}

int isInteger(Node* head) {
    double value;
    get_numeric(head, &value);
    return (value - floorl(value) < GLOBAL_THRESHOLD);
}

int isSign(Node* head) {
    return (head->type == lt_plus ||  head->type == lt_minus);
}