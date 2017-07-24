/*
 * File:   main.c
 * Author: warford
 *
 * Created on June 14, 2017, 7:50 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to generate Eventual latching counterexample
 * Conclusion: Eventual latching is a theorem.
 * Cannot use Methatheorem Eventually, because without the eventually operators, it is not a theorem.
 */

const int numStates = 10;

int p[numStates];
int q[numStates];

int always_q[numStates];
int p_implies_always_q[numStates];
int always_p_implies_always_q[numStates];
int event_always_p_implies_always_q[numStates];
int not_p[numStates];
int always_not_p[numStates];
int always_q_or_always_not_p[numStates];
int event_always_q_or_always_not_p[numStates];
int event_always_p_implies_always_q_implies_event_always_q_or_always_not_p[numStates];

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
    int k;
    for (int j = 0; j < numStates; j++) {
        k = j;
        while (k < numStates && first[k] == 1) {
            k++;
        }
        result[j] = (k == numStates ? 1 : 0);
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
    }
    for (int i = 0; i < numStates2; i++) {
        for (int j = 0; j < numStates2; j++) {
            for (int m = 0; m < numStates; m++) {
                always(q, always_q);
                implies(p, always_q, p_implies_always_q);
                always(p_implies_always_q, always_p_implies_always_q);
                event(always_p_implies_always_q, event_always_p_implies_always_q);
                not(p, not_p);
                always(not_p, always_not_p);
                or(always_q, always_not_p, always_q_or_always_not_p);
                event(always_q_or_always_not_p, event_always_q_or_always_not_p);
                implies(event_always_p_implies_always_q, event_always_q_or_always_not_p, event_always_p_implies_always_q_implies_event_always_q_or_always_not_p);
                if (event_always_p_implies_always_q_implies_event_always_q_or_always_not_p[m] == 0) {
                    printf("Counterexample found\n");
                    printf("                        p: ");
                    printStates(p);
                    printf("                        q: ");
                    printStates(q);
                    printf("                     Al q: ");
                    printStates(always_q);
                    printf("           p implies Al q: ");
                    printStates(p_implies_always_q);
                    printf("      Al (p implies Al q): ");
                    printStates(always_p_implies_always_q);
                    printf("                 Al not p: ");
                    printStates(always_not_p);
                    printf("         Al q or Al not p: ");
                    printStates(always_q_or_always_not_p);
                    printf("\n");
                }
            }
            nextState(q);
        }
        nextState(p);
    }
    return (EXIT_SUCCESS);
}
