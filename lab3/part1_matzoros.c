#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) { //ASKASK is there something that needs to get placed in here?
	int child = fork(); //forking child 1
	if (child < 0) //checks if there was an error
		perror("fork"); //prints if there was

	if (child == 0) { //if there was no error
		printf("\nChild is created with pid value : %d and the date and time is : \n", getpid()); //creates child and prints PID 
		execl("/bin/date",argv[0],NULL); //prints the time
		printf("FAILURE TO GET TIME AND DATE \n"); //if it gets here the execv failed!
	}
	else
		printf("\nChild with PID value: %d has finished\n",waitpid(child,NULL,0)); //print the child PID that was terminated
	return 0;
}
