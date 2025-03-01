#include "./constants.h"

int c_constant(char** s, int* constant_type) {
    if(c_decimal(s)) {*constant_type = ct_decimal; goto P;}
    if(c_number(s)) {*constant_type = ct_number; goto P;}
    if(c_compare(s, c_zero, 1)) {*constant_type = ct_zero; goto P;}
    return 0;
P:  return 1;
}

int c_compare(char** s, char hash[][3], int keyCount) {
    for(int i = 0; i < keyCount; i++) {
        if(strncmp(*s, hash[i], strlen(hash[i]))==0) {
            *s += strlen(hash[i]);
            c_F_compare = i+1; return 1;
        }
    } c_F_compare = 0; return 0;
}

int c_number(char** s) {
    char* ps = *s;
    if(!c_compare(s, c_nonzero, 9)) goto f;
    while(c_digit(s));
    goto t;
f : *s = ps;
    return 0;
t : return 1;
}
int c_decimal(char** s) {
    char* ps = *s;
    if(!c_compare(s, c_nonzero, 9)) goto f;
    while(c_digit(s));
    if(!c_compare(s, c_period, 1)) goto f;
    while(c_digit(s));  
    goto t;
f : *s = ps;
    return 0;
t : return 1;
}
int c_digit(char** s) {
    char* ps = *s;
    if(!c_compare(s, c_zero, 1)) goto c2;
    goto t;
c2: *s = ps;
    if(!c_compare(s, c_nonzero, 9)) goto f;
    goto t;
f : *s = ps;
    return 0;
t : return 1;
}


char c_period[1][3] = {"."};
char c_nonzero[9][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
char c_zero[1][3] = {"0"};
