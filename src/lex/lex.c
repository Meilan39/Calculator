#include "lex.h"

int l_lex(Token* this, char* s) {
    char* ps;
    int type;
    while(*s!='\0') {
        // pass spaces
        while(*s==' ') {s++;}
        if(*s=='\0') {break;}
        // general init
        ps = s;
        type = 0;
        // numerical constants
        if(c_constant(&s, &type)) {goto P;}  
        if( (type = l_hash(&s)) ) {goto P;}
        // lexical error
        return -1;
    P:  // pushback
        strncpy(l_buffer, ps, s-ps);
        l_buffer[s-ps] = '\0';
        t_push(this, l_buffer, type);
    } return 0;
}

// int l_compare(char** s, char hash[][20], int keyCount) {
//     for(int i = 0; i < keyCount; i++) {
//         if(strncmp(*s, hash[i], strlen(hash[i]))==0) {
//             *s += strlen(hash[i]);
//             return 1;
//         }
//     } return 0;
// }

int l_hash(char** s) {
    const char* value;
    for(int i = 0; value = l_map[i].value; i++) {
        if(strncmp(*s, value, strlen(value)) == 0) {
            *s += strlen(value);
            return l_map[i].key;
        }
    } return 0;
}

char* l_unhash(int key) {
    for(int i = 0; l_map[i].key; i++) {
        if(key == l_map[i].key) {
            return l_map[i].value;
        }
    } return NULL;
}

const l_Map l_map[] = {
    /* common symbols */
    {"+", lt_plus},
    {"-", lt_minus},
    {"*", lt_dot},
    {"/", lt_slash},
    {"^", lt_caret},
    {"=", lt_equal},
    {"E", lt_scientific},
    {"(", lt_h_parenthesis},
    {")", lt_t_parenthesis},
    {"{", lt_h_bracket},
    {"}", lt_t_bracket},
    /* specific symbols */
    {"e", lt_e},
    {"pi", lt_pi},
    {"x", lt_x},
    /* commands */
    {"root", lt_root},
    /* end */
    {NULL, 0}
};