/*
 * File:   main.c
 * Author: warford
 *
 * Created on May 30, 2018, 11:08 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to generate counterexample of an Until absorption theorem
 * Conclusion: Appears to be a theorem
 * Yes, now proved.
 */

const int numStates = 11;

int p[numStates];
int q[numStates];

int event_q[numStates];
int p_until_q[numStates];
int p_until_q_or_event_q[numStates];
int event_q_implies_p_until_q_or_event_q[numStates];
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

void event(int *first, int *result) {
    int k;
    for (int j = 0; j < numStates; j++) {
        k = j;
        while (k < numStates && first[k] == 0) {
            k++;
        }
        result[j] = (k == numStates ? 0 : 1);
    }
}

void or(int *first, int *second, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 1 || second[j] == 1 ? 1 : 0);
    }
}

void implies(int *first, int *second, int *result) {
    for (int j = 0; j < numStates; j++) {
        result[j] = (first[j] == 1 && second[j] == 0 ? 0 : 1);
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
    }
    for (int i = 0; i < numStates2; i++) {
        for (int j = 0; j < numStates2; j++) {
            for (int m = 0; m < numStates; m++) {
                event(q, event_q);
                until(p, q, p_until_q);
                or(p_until_q, event_q, p_until_q_or_event_q);
                implies(event_q, p_until_q_or_event_q, event_q_implies_p_until_q_or_event_q);
                if (event_q_implies_p_until_q_or_event_q[m] == 0) {
                    printf("Counterexample found\n");
                    printf("                          p: ");
                    printStates(p);
                    printf("                          q: ");
                    printStates(q);
                    printf("                       Ev q: ");
                    printStates(event_q);
                    printf("                     p Un q: ");
                    printStates(p_until_q);
                    printf("             p Un q or Ev q: ");
                    printStates(p_until_q_or_event_q);
                    printf("Ev q implies p Un q or Ev q: ");
                    printStates(event_q_implies_p_until_q_or_event_q);
                    printf("\n");
                }
            }
            nextState(q);
        }
        nextState(p);
    }
    return (EXIT_SUCCESS);
}
