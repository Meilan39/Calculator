#include "root.h"

int root_resolve(Node* head) {
    if(head->next[0]->type != nt_root) return 0;
    int degree;
    if(root_degree(head->next[0]->next[1], &degree) == -1) goto E;
    switch(degree) {
        case 0:
            printf("...no variable to calculate\n");
            goto E;
        case 1: 
            printf(" > 0\n");
            break;
        case 2: root_second_degree(head); break;
        case 3: root_third_degree(head); break;
        default:
    }
    return 1;
E:  return -1;
}

int root_degree(Node* head, int* degree) {
    *degree = 0;
    for(int i = 0; i < head->length; i++) {
        Node* next = head->next[i];
        int temp;
        if(next->type != nt_polynomial_term) continue;
        if(root_get_degree(next, &temp) == -1) goto E;
        *degree = fmax(*degree, temp);
    }
    return 0;
E:  return -1;
}

int root_get_degree(Node* head, int* degree) {
    if(head->length == 1) { *degree = 0; }
    if(head->length == 2) { *degree = 1; }
    if(head->length == 4) {
        long double value;
        if(get_numeric(head->next[3], &value) == -1) goto E;
        *degree = value;
    }
    return 0;
E:  return -1;
}

int root_coefficient(Node* head, long double* coefficients, int degree) {
    for(int i = degree; i >= 0; i++) {
        for(int j = 0; j < head->length; j++) {
            Node* next = head->next[j];
            long double value;
            int temp;
            if(root_get_degree(next, &temp) == -1) goto E;
            if(temp != i) continue;
            if(get_numeric(next->next[0], &value) == -1) goto E;
            coefficients[degree-i] += value;
        }
    }
    return 0;
E:  return -1;
}

int root_second_degree(Node* head) {
    int degree = 2;
    Node* X1, X2;
    long double x1, x2;
    long double* coefficients = calloc(degree, sizeof(long double));
    if(root_coefficient(head, coefficients, degree) == -1) goto E;
    printf(" │ real roots: %Lf, %Lf\n", x1, x2);
    free(coefficients);
    return 0;
E:  return -1;
}

int root_third_degree(Node* head) {
    return 0;
E:  return -1;
}