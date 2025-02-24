#include "lex.h"

int l_lex(Token* this, char* s) {
    char* ps;
    int type;
    while(*s!='\0') {
        // pass spaces
        while(*s==' ') {s++;}
        // general init
        ps = s;
        type = 0;
        // checks
        if(l_compare(&s, l_special_symbols, 2)) {type = lt_special_symbols; goto P;}
        if(l_compare(&s, l_root, 1)) {type = lt_root; goto P;}        
        if(l_compare(&s, l_plus, 1)) {type = lt_plus; goto P;}
        if(l_compare(&s, l_minus, 1)) {type = lt_minus; goto P;}
        if(l_compare(&s, l_dot, 1)) {type = lt_dot; goto P;}
        if(l_compare(&s, l_slash, 1)) {type = lt_slash; goto P;}
        if(l_compare(&s, l_caret, 1)) {type = lt_caret; goto P;}
        if(l_compare(&s, l_equal, 1)) {type = lt_equal; goto P;}
        if(l_compare(&s, l_period, 1)) {type = lt_period; goto P;}
        if(l_compare(&s, l_scientific, 1)) {type = lt_scientific; goto P;}
        if(l_compare(&s, l_h_parenthesis, 1)) {type = lt_h_parenthesis; goto P;}
        if(l_compare(&s, l_t_parenthesis, 1)) {type = lt_t_parenthesis; goto P;}
        if(l_compare(&s, l_h_bracket, 1)) {type = lt_h_bracket; goto P;}
        if(l_compare(&s, l_t_bracket, 1)) {type = lt_t_bracket; goto P;}
        if(l_compare(&s, l_nonzero, 9)) {type = lt_nonzero; goto P;}
        if(l_compare(&s, l_zero, 1)) {type = lt_zero; goto P;}
        if(l_compare(&s, l_variable, 26)) {type = lt_variable; goto P;}
        // lexical error
        return -1;
    P:  // pushback
        strncpy(l_buffer, ps, s-ps);
        l_buffer[s-ps] = '\0';
        t_push(this, l_buffer, type);
    } return 0;
}

int l_compare(char** s, char hash[][20], int keyCount) {
    for(int i = 0; i < keyCount; i++) {
        if(strncmp(*s, hash[i], strlen(hash[i]))==0) {
            *s += strlen(hash[i]);
            return 1;
        }
    } return 0;
}

char l_plus[1][20] = {"+"};
char l_minus[1][20] = {"-"};
char l_dot[1][20] = {"*"};
char l_slash[1][20] = {"/"};
char l_caret[1][20] = {"^"};
char l_equal[1][20] = {"="};
char l_period[1][20] = {"."};
char l_scientific[1][20] = {"E"};
char l_h_parenthesis[1][20] = {"("};
char l_t_parenthesis[1][20] = {")"};
char l_h_bracket[1][20] = {"{"};
char l_t_bracket[1][20] = {"}"};
char l_nonzero[9][20] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
char l_zero[1][20] = {"0"};
char l_variable[26][20] = {"a","b","c","d","e","f","g","h","i","j","k","l","m",
                           "n","o","p","q","r","s","t","u","v","w","x","y","z"};

char l_special_symbols[2][20] = {"\\e", "\\pi"};
char l_root[1][20] = {"root"};