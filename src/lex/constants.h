#ifndef CONSTANTS
#define CONSTANTS

#include <string.h>

#include "../token/token.h"

typedef enum c_Types c_Types;

int c_F_compare; // flag for constant_compare

int c_constant(char** s, int* constant_type);

int c_compare(char** s, char hash[][3], int keyCount);

int c_number(char** s);
int c_decimal(char** s);
int c_digit(char** s);
int c_sign(char** s);

char c_period[1][3];
char c_nonzero[9][3];
char c_zero[1][3];

enum c_Types {
    ct_number = 100,
    ct_decimal,
    ct_zero
};

#endif