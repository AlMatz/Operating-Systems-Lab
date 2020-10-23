#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) { 
	int child = fork(); //forking child 1
	if (child < 0) //chckes for error
		perror("fork"); //if there is error print it

	char* args[3]; //sets args to be used 
	args[0] = "ls"; //ls to look in a file 
	args[1] = "-la"; //to display file info and hidden files 
	args[2] = NULL; //end should be null for execvp
	if (child == 0) { //if there was no error
		printf("\nChild is created with pid value : %d and the files are listed below : \n", getpid()); //print child with PID
		execvp("ls",args); //execute the commands above
		printf("FAILURE TO PRINT FILES \n"); //if it gets here the execv failed!
	}
	else {
		printf("\n Child terminated PID is : %d\n",waitpid(child,NULL,0)); //print PID of finished child
	}
	return 0;
}
