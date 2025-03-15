#ifndef LEX
#define LEX

#include <string.h>
#include "../token/token.h"
#include "./constants.h"

typedef struct l_Map l_Map;
typedef enum l_Types l_Types;
char l_buffer[256];

struct l_Map {
    const char* value;
    const int key;
};
extern const l_Map l_map[];

int l_lex(Token* this, char* s);

int l_hash(char** s);
const char* l_unhash(int key);

enum l_Types {
    /* common symbols */
    lt_plus = ct_terminator + 1, 
    lt_minus, 
    lt_dot, 
    lt_slash,
    lt_caret,
    lt_equal,
    lt_comma,
    lt_scientific,
    lt_h_parenthesis,
    lt_t_parenthesis,
    lt_h_bracket,
    lt_t_bracket,
    /* specific symbols */
    lt_e,
    lt_pi,
    lt_x,    
    /* commands */
    lt_zeros,
    /* fuinctions */
    lt_root,
    lt_sqrt,
    lt_log,
    lt_ln,
    lt_sin,
    lt_cos,
    lt_tan,
    lt_asin,
    lt_acos,
    lt_atan,
    lt_sinh,
    lt_cosh,
    lt_tanh,
    /* terminator */
    lt_terminator
};

#endif