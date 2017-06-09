/*
 * File:   main.c
 * Author: warford
 *
 * Created on June 9, 2017, 11:28 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to generate And Until counterexample
 */

const int numStates = 8;

int p[numStates];
int q[numStates];
int r[numStates];
int p_and_q[numStates];
int q_until_r[numStates];
int pandq_until_r[numStates];
int p_until_quntilr[numStates];

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

void and(int *first, int *second, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 1 && second[j] == 1 ? 1 : 0);
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
                    and(p, q, p_and_q);
                    until(q, r, q_until_r);
                    until(p_and_q, r, pandq_until_r);
                    until(p, q_until_r, p_until_quntilr);
                    if (pandq_until_r[m] == 1 && p_until_quntilr[m] == 0) {
                        printf("                  p: ");
                        printStates(p);
                        printf("                  q: ");
                        printStates(q);
                        printf("                  r: ");
                        printStates(r);
                        printf("            p And q: ");
                        printStates(p_and_q);
                        printf("          q Until r: ");
                        printStates(q_until_r);
                        printf("  (p And q) Until r: ");
                        printStates(pandq_until_r);
                        printf("p Until (q Until r): ");
                        printStates(p_until_quntilr);
                        printf("\n");
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
