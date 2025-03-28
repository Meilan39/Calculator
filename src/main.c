#include <stdio.h>
#include <stdlib.h>

#include "./token/token.h"
#include "./tree/tree.h"
#include "./lex/lex.h"
#include "./syn/syn.h"
#include "./resolve/resolve.h"

#define BUFFER_LENGTH 256

char buffer[BUFFER_LENGTH];

void fprint(const char* filename);

int main(void) {
    int exit = 0;
    while(!exit) {
        // construct tokens and objects
        Token* tokens = t_construct();
        Node* head = NULL;
        // get stdin
        printf(" > ");
        fgets(buffer, BUFFER_LENGTH, stdin);
        // terminate
        buffer[BUFFER_LENGTH - 1] = '\n';
        buffer[strcspn(buffer, "\n")] = '\0';
        // stdin check
        if(buffer[0]=='\0') {goto E;}
        if(strcmp("exit", buffer)==0) {printf("...exiting\n"); exit = 1; goto E;}
        if(strcmp("help", buffer)==0) {fprint("./src/files/help.txt"); goto E;}
        // lexical analysis
        if(l_lex(tokens, buffer)) {printf("...lexical error\n"); goto E;}
        // syntax analysis
        if(s_syn(&head, tokens->next)) {printf("...syntax error\n"); goto E;}
        // syntaz tree
        n_simplify(head);
        // resolve
        resolve(head);
    E:  // close file
        t_destruct(tokens);
        n_free(head);
    }
    return 0;
}

void fprint(const char* filename) {
    FILE* fptr = fopen(filename, "r");
    if(fptr==NULL) {printf("unable to open file\n"); goto E;}
    char fileBuffer[BUFFER_LENGTH];
    while (fgets(fileBuffer, BUFFER_LENGTH, fptr) != NULL) {
        printf("%s", fileBuffer);
    }
E:  fclose(fptr);
}
