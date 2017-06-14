/*
 * File:   main.c
 * Author: warford
 *
 * Created on June 14, 2017, 2:28 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to test Wait ordering theorem.
 */

const int numStates = 8;

int p[numStates];
int q[numStates];
int r[numStates];

int p_wait_q[numStates];
int not_q[numStates];
int not_q_wait_r[numStates];
int p_wait_q_and_not_q_wait_r[numStates];
int p_wait_r[numStates];

int temp1[numStates]; // global for efficiency
int temp2[numStates];

void printStates(int *s) {
    for (int i = 0; i < numStates; i++) {
        printf("%d ", s[i]);
    }
    printf("\n");
}

void not(int *first, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 0 ? 1 : 0);
    }
}

void always(int *first, int *result) {
    int k, m;
    for (int j = 0; j < numStates; j++) {
        while (k < numStates && first[j] == 1) {
            k++;
        }
        result[j] = (k == numStates ? 1 : 0);
    }
}

void and(int *first, int *second, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 1 && second[j] == 1 ? 1 : 0);
    }
}

void or(int *first, int *second, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 1 || second[j] == 1 ? 1 : 0);
    }
}

void until(int *first, int *second, int *result) {
    int i, k;
    for (int j = 0; j < numStates; j++) {
        if (second[j] == 1) {
            result[j] = 1; // True is the right zero of Until
        } else {
            k = j; // Find index of first occurrence of second
            while (k < numStates && second[k] == 0) {
                k++;
            }
            result[j] = 0;
            if (k < numStates) {
                i = j;
                while (i < k && first[i] == 1) {
                    i++;
                }
                result[j] = (i == k ? 1 : 0);
            }
        }
    }
}

void myWait(int *first, int *second, int *result) {
    always(first, temp1);
    until(first, second, temp2);
    or(temp1, temp2, result);
}

int main(int argc, char** argv) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 1;
    p[3] = 1;
    p[4] = 1;
    p[5] = 1;
    p[6] = 1;
    p[7] = 0;
    q[0] = 0;
    q[1] = 1;
    q[2] = 0;
    q[3] = 1;
    q[4] = 1;
    q[5] = 1;
    q[6] = 1;
    q[7] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 1;
    r[4] = 1;
    r[5] = 1;
    r[6] = 1;
    r[7] = 0;
    myWait(p, q, p_wait_q);
    not(q, not_q);
    myWait(not_q, r, not_q_wait_r);
    and(p_wait_q, not_q_wait_r, p_wait_q_and_not_q_wait_r);
    myWait(p, r, p_wait_r);
    printf("                        p: ");
    printStates(p);
    printf("                        q: ");
    printStates(q);
    printf("                        r: ");
    printStates(r);
    printf("                 p Wait q: ");
    printStates(p_wait_q);
    printf("                    Not q: ");
    printStates(not_q);
    printf("             Not q Wait r: ");
    printStates(not_q_wait_r);
    printf("p Wait q And Not q Wait r: ");
    printStates(p_wait_q_and_not_q_wait_r);
    printf("                 p Wait r: ");
    printStates(p_wait_r);
    return (EXIT_SUCCESS);
}
