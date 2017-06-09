/*
 * File:   main.c
 * Author: warford
 *
 * Created on June 9, 2017, 10:59 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Program to test and() and until().
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
   p[0] = 0; p[1] = 0; p[2] = 1; p[3] = 1; p[4] = 1; p[5] = 1; p[6] = 0; p[7] = 0;
   q[0] = 0; q[1] = 1; q[2] = 0; q[3] = 1; q[4] = 0; q[5] = 0; q[6] = 0; q[7] = 0;
   r[0] = 0; r[1] = 0; r[2] = 0; r[3] = 1; r[4] = 1; r[5] = 0; r[6] = 1; r[7] = 0;
   and(p, q, p_and_q);
   until(q, r, q_until_r);
   until(p_and_q, r, pandq_until_r);
   until(p, q_until_r, p_until_quntilr);
   printf("                  p: "); printStates(p);
   printf("                  q: "); printStates(q);
   printf("                  r: "); printStates(r);
   printf("            p And q: "); printStates(p_and_q);
   printf("          q Until r: "); printStates(q_until_r);
   printf("  (p And q) Until r: "); printStates(pandq_until_r);
   printf("p Until (q Until r): "); printStates(p_until_quntilr);
   return (EXIT_SUCCESS);
}
