/**********************************************************************************

           *************NOTE**************
This is a template for the subject of RTOS in University of Technology Sydney(UTS)
Please complete the code based on the assignment requirement.

Assignment 3 Program_2 template

**********************************************************************************/
/*
  To compile prog_1 ensure that gcc is installed and run the following command:
  gcc -Wall -O2 program_2.c -o prog_2 -lpthread -lrt

*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "queue.h"

//Number of pagefaults in the program
int pageFaults = 0;

//size of the frame by default set to 4
int frameSize = 4;

//if user has entered ctrl+C
bool waitingForUser = true;

//Function declaration
void SignalHandler(int signal);
//void printFrame(int pageFaults);

/**
 Main routine for the program. In charge of setting up threads and the FIFO.

 @param argc Number of arguments passed to the program.
 @param argv array of values passed to the program.
 @return returns 0 upon completion.
 */

int main(int argc, char* argv[])
{
	//Register Ctrl+c(SIGINT) signal and call the signal handler for the function.
	signal(SIGINT, SignalHandler);
	
	//if user wants to specify the frame size
	//checks if user input is a valid integer
	//prints error message and exits program if user enters a non-integer value as the frameSize
	if (argv[1]!= NULL){
		frameSize = atoi(argv[1]);
		if (frameSize < 1){
			printf("Error: No valid number has been entered\n");
			exit(0);
		}
	}
	
	int i;
	// reference number
	int REFERENCESTRINGLENGTH=24;
	//Frame where we will be storing the references. -1 is equivalent to an empty value
	int frame[frameSize];
	//Reference string from the assignment outline
	int referenceString[24] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1,7,5};
	//Next position to write a new value to.
	//int nextWritePosition = 0;
	//Boolean value for whether there is a match or not.
	bool match = false;
	//Current value of the reference string.
	int currentValue;

	//Initialise the empty frame with -1 to simulate empty values.
	for(i = 0; i < frameSize; i++)
	{
		frame[i] = -1;
	}

	//Creating a queue
	struct Queue* queue = createQueue(REFERENCESTRINGLENGTH);

	//Loop through the reference string values and check for empty space in the frame.
	for(i = 0; i < REFERENCESTRINGLENGTH; i++)
	{
		match = false;
		currentValue = referenceString[i];

		//checking to see if value already exists in the frame
		for (int x = 0; x < frameSize; x++){
			if(frame[x] == currentValue){
				match = true;
			}
		}

		if(match == false){
			bool freeSpace = false;
			//printf("The current value is: %i\n", currentValue);
			for(int j = 0; j < frameSize; j++){
				if(frame[j] == -1){
					freeSpace = true;
					frame[j] = currentValue;
					enqueue(queue, currentValue);
					pageFaults++;
					break;
				}
			}

			//After iterating through the frame, determining that there is no free space 
			//and still wanting to add the current (new) value
			if (freeSpace == false){
				int oldestValue = dequeue(queue);

				//find the position of the oldest value
				for(int y = 0; y < frameSize; y++){
					if(frame[y] == oldestValue){
						frame[y] = currentValue;
						enqueue(queue, currentValue);
						pageFaults++;
						break;
					}
				}
			
			}
		}
		printf("Current page fault: %d\n", pageFaults);
		//for (i = 0; i < frameSize; i++){
		printf("State of current frame: %d\n", currentValue);
		//}
	}
	
	//Sit here until the ctrl+c signal is given by the user.
	while(waitingForUser)
	{
		sleep(1);
	}

	printf("\nTotal frame size is: %d\n", frameSize);
	printf("Total page faults: %d\n", pageFaults);

	return 0;
}

/**
 Performs the final print when the signal is received by the program.

 @param signal An integer values for the signal passed to the function.
 */
void SignalHandler(int signal)
{
	waitingForUser = false;
}

//function for printing pagefault for each iteration while program is running
//void printFrame(int pageFaults)
//{
//	printf("Total page faults: %d\n", pageFaults);
//}