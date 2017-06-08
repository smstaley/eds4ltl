/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: warford
 *
 * Created on June 7, 2017, 7:19 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

const int numStates = 4;
int p[numStates];
int q[numStates];
int p_until_q[numStates];

void printStates(int *s) {
   for (int i = 0; i < numStates; i++) {
      printf("%d ", s[i]);
   }
   printf("\n");
}

int main(int argc, char** argv) {
   p[0] = 0;
   p[1] = 1;
   p[2] = 1;
   p[3] = 0;
   q[0] = 1;
   q[1] = 1;
   q[2] = 0;
   q[3] = 0;
   printf("     p: "); printStates(p);
   printf("     q: "); printStates(q);
   return (EXIT_SUCCESS);
}

