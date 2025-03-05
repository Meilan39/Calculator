#ifndef LEX
#define LEX

#include <string.h>
#include "../token/token.h"
#include "./constants.h"

typedef enum l_Types l_Types;
char l_buffer[256];

int l_lex(Token* this, char* s);
int l_compare(char** s, char hash[][20], int keyCount);

char l_plus[1][20];
char l_minus[1][20];
char l_dot[1][20];
char l_slash[1][20];
char l_caret[1][20];
char l_equal[1][20];
char l_scientific[1][20];
char l_h_parenthesis[1][20];
char l_t_parenthesis[1][20];
char l_h_bracket[1][20];
char l_t_bracket[1][20];
char l_variable[26][20];
char l_special_e[1][20];
char l_special_pi[1][20];
char l_root[1][20];

enum l_Types {
    /* common symbols */
    lt_plus = 1, 
    lt_minus, 
    lt_dot, 
    lt_slash,
    lt_caret,
    lt_equal,
    lt_scientific,
    lt_h_parenthesis,
    lt_t_parenthesis,
    lt_h_bracket,
    lt_t_bracket,
    lt_variable,
    /* specific symbols */
    lt_special_e,
    lt_special_pi,
    lt_root
};

#endif