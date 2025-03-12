#include "global.h"

const long double GLOBAL_E = 2.71828182;
const long double GLOBAL_PI = 3.14159265;
const long double FORMAT_THRESHOLD = 0.00000001;

int get_numeric(Node* head, long double* value) {
    long double a = 1;
    int hasSign;
    errno = 0;
    /* convert */
    if(head->type == nt_special_symbols) {
        hasSign = head->length - 1;
        if(head->next[hasSign]->type == lt_e) a = GLOBAL_E;
        if(head->next[hasSign]->type == lt_pi) a = GLOBAL_PI;
        if(head->next[0]->type == lt_minus) a = -a;
    }
    if(head->type == nt_scientific) {
        /* exponent part */
        Node* exponent = head->next[2];
        int temp;
        hasSign = exponent->length - 1;
        temp = exponent->next[hasSign]->value;
        if(exponent->next[0]->type == lt_minus) temp = -temp;        
        a *= powl(10, temp);
        /* rational part parsed next */
        head = head->next[0];        
    }
    if(head->type == nt_rational || head->type == nt_integer || head->type == nt_natural) {
        /* rational, integer, natural */
        hasSign = head->length - 1;
        a *= head->next[hasSign]->value;
        if(head->next[0]->type == lt_minus) a = -a;          
    }
    *value = a;
    /* range */
    if(errno == ERANGE || !isfinite(a)) return -1;
    else return 0;
}

// long double set_numeric(Node* head);

// int print_numeric_map(Node* head);
