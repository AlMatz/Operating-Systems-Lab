#include <stdio.h>
int main() {
	printf("\nI am the parent, my pid is: %d \n",getpid()); //prints parent PID
	int child1 = fork(); //forks the first child
	//checking for error in forking and prints the error if there is
	if (child1 < 0)
		perror("fork");

	//no error in forking
	if (child1 == 0) {
		printf("\nI am the first child, my pid is: %d \n",getpid()); //prints pid
		execl("Process_P1","Process_P1",NULL); //executes the first process process_P1
	}
	else {
		//sleep(3);
		printf("\nChild1 has finished with PID: %d\n",waitpid(child1,NULL,0)); //prints that the child is finished and the PID
		int child2 = fork(); //forks the 2nd child 
		if (child2 < 0) //checks if there is error in forking
			perror("fork"); //prints the error if there is

		if (child2 == 0) { //if there isnt an error
			printf("\nI am the second child, my pid is: %d \n",getpid()); //prints the second child's PID
			execl("Process_P2","Process_P2",NULL); //executes the second process Process_P2
		}
		else
		{
			printf("\nChild2 ended with PID : %d \n",waitpid(child2,NULL,0)); //prints that the second child is finished and its PID
		}


	}
	return 0;
}

