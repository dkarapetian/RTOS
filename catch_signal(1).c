/***********************
   ----- 48450 -- week 8 lab handout 1 ------ 
This is a program to practice register and catch ctrl+c signal
**********************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// declare the function
void sighandler(int);

// record the result of the calculation
int result=0;
int frameSize = 4;


int main (int argc, char * argv[]) {
   if (atoi(argv[1])!= 0)
      {frameSize = atoi(argv[1]);}
   // declare the variable
   int i=0;

   printf("Frame size: %i", frameSize);

   /* register a signal for Ctrl+c (SIGINT: signal interrupt signal)*/
   signal(SIGINT, sighandler);
   
   printf("\nSignal registration finish. Algorithm begin...\n");
   // algorithm   
   for(i=0;i<100;i++)
        result=result+i;
   printf("Algorithm finish, waiting for Ctrl+c signal...\n");
   /*loop until there is a interrupt signal (Ctrl+c) come in*/
   while(1) {
      sleep(1); 
   }
   return(0);
}

void sighandler(int signum) {
 
   printf("\n Catch ctrl+c signal. The result of the algorithm is %d. \n",result);
   exit(1);
}
