/*
 * File:   main.c
 * Author: warford
 *
 * Created on June 14, 2017, 7:50 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to generate Wait Ordering counterexample
 */

const int numStates = 7;

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

void nextState(int *s) {
    int carryIn = (s[numStates - 1] + 1) / 2;
    int carryOut;
    s[numStates - 1] = (s[numStates - 1] + 1) % 2;
    for (int j = numStates - 2; 0 <= j; j--) {
        carryOut = (s[j] + carryIn) / 2;
        s[j] = (s[j] + carryIn) % 2;
        carryIn = carryOut; // Re-establish loop invariant for next column
    }
}

void not(int *first, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 0 ? 1 : 0);
    }
}

void always(int *first, int *result) {
    int k, m;
    for (int j = 0; j < numStates; j++) {
        k = j;
        while (k < numStates && first[k] == 1) {
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
    /*
    printf("\n\nfirst:\n");
    printStates(first);
    printf("Always first:\n");
    printStates(temp1);
    printf("second:\n");
    printStates(second);
    printf("first Until second:\n");
    printStates(temp2);
    printf("Alway forst Or first Until second:\n");
    printStates(result);
     */
}

int main(int argc, char** argv) {
    int numStates2 = 1 << numStates;
    printf("numStates == %d, numStates2 == %d\n\n", numStates, numStates2);
    for (int i = 0; i < numStates; i++) {
        p[i] = 0;
        q[i] = 0;
        r[i] = 0;
    }
    for (int i = 0; i < numStates2; i++) {
        for (int j = 0; j < numStates2; j++) {
            for (int k = 0; k < numStates2; k++) {
                for (int m = 0; m < numStates; m++) {
                    myWait(p, q, p_wait_q);
                    not(q, not_q);
                    myWait(not_q, r, not_q_wait_r);
                    and(p_wait_q, not_q_wait_r, p_wait_q_and_not_q_wait_r);
                    myWait(p, r, p_wait_r);
                    if (p_wait_q_and_not_q_wait_r[m] == 1 && p_wait_r[m] == 0) {
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
                    }
                }
                nextState(r);
            }
            nextState(q);
        }
        nextState(p);
    }
    return (EXIT_SUCCESS);
}
