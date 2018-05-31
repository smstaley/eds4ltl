/*
 * File:   main.c
 * Author: warford
 *
 * Created on May 30, 2018, 2:04 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to generate counterexample of an Always Wait theorem
 * Conclusion: Obviously not a theorem
 */
const int numStates = 4;

int p[numStates];
int q[numStates];

int always_p[numStates];
int always_p_wait_q[numStates];
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
    int numStates2 = 1 << numStates;
    printf("numStates == %d, numStates2 == %d\n\n", numStates, numStates2);
    for (int i = 0; i < numStates; i++) {
        p[i] = 0;
        q[i] = 0;
    }
    for (int i = 0; i < numStates2; i++) {
        for (int j = 0; j < numStates2; j++) {
            for (int m = 0; m < numStates; m++) {
                always(p, always_p);
                myWait(always_p, q, always_p_wait_q);
                if (always_p_wait_q[m] != always_p[m]) {
                    printf("Counterexample found\n");
                    printf("                          p: ");
                    printStates(p);
                    printf("                          q: ");
                    printStates(q);
                    printf("                       Al p: ");
                    printStates(always_p);
                    printf("                  Al p Wt q: ");
                    printStates(always_p_wait_q);
                    printf("\n");
                }
            }
            nextState(q);
        }
        nextState(p);
    }
    return (EXIT_SUCCESS);
}

