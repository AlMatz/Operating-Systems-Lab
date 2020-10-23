#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	char cmd_buff[256]; //buffer for typing
	char *ptr; //ptr for string split
	size_t length = 256; //length of buff
	char *args[15]; //to holds arguments
	int i = 0; //iterating
	int child1; //child that will be forked
	while(1) {
		i = 0; //reset iterator
		printf("\nPlease input the cmd you would like to run: "); //state what to do
		fgets(cmd_buff,length,stdin); //gets what is written 
		ptr = strtok(cmd_buff," \n"); //will be used to break the string 
		memset(args,0,sizeof(args)); //resets the contents of args to empty
		while (ptr != NULL) { //a while loop to continue till we get to the end of ptr
			args[i] = ptr; //sets the args to whatever ptr is pointing to
			if (strcmp(args[0],"quit") ==0) //if they wrote quit, it quits the program
				exit(0); //exits
			ptr = strtok(NULL," \n"); //continues iterating
			i++; //increments i to write to the next location of args
		}
	args[i] = NULL; //sets the last thing in args to NULL for execvp
	child1 = fork(); //forks the child
	if(child1 < 0)  //check if error
		perror("fork"); //if error print it
	if(child1 == 0) {	 //if works correctly
		execvp(args[0],args); //execute it
		perror("execvp");//if error occured print it
		exit(1); //exit 
	}
	else
	printf("\nChild1 terminated with pid: %d\n",waitpid(child1,NULL,0)); //wait for program to end to print PID
	}
return 0; //return when we are done.
}
