#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	int x = 3; //# of chapters 
	int y = 2; //hws for each chapter
	int students = 4; //how many students
	char* grades_file = "grades.txt";  //cotains the grades in the file.
	FILE *file = fopen(grades_file,"r"); //opens file in grades_file (source that contains grades) with read perms
	int grades[students][x*y]; //2D array 
	if(file == NULL) { //check if it doesnt open
		perror("fopen"); //print error
		exit(1); //exit
	}
	
	for(int col = 0; col < (students); col++) { //to read the rows 
		for(int row = 0; row < x*y; row++) { // to read each column
			fscanf(file,"%d",&grades[col][row]); //will hold the grades read from the file
			printf("%d ",grades[col][row]);	 //prints the grades for user to see it
		}
		printf("\n"); //prints new line to give it box like structure
	}
	FILE *closing = fclose(file); //closes file and checks for errors
	if (closing != NULL) { //is there error
		perror("fclose"); //if error print it 
		exit(1); //exit
	}	
	
	int managers[x]; //there are x managers
	int workers[x*y]; //there are x*y workers in total
	float averages[x*y]; //will hold averages for each hw
	int max[x*y]; //will hold max for each hw
	for(int i = 0; i < x; i++) { //for managers
		managers[i] = fork(); //fork a manager
		//printf("i=%d\n",i); //uncoment to print the current manager
		if (managers[i] < 0) //if it doesnt work
			perror("fork"); //print error
		if (managers[i] == 0) { //if the fork works correctly we have managers
			for(int j = 0; j < y; j++) { //now time for workers
				//printf("j=%d\n",j); //un comment to print the current worker for the manager
				workers[j+(i*y)] = fork(); //j+(i*y) gets the correct location in workers array, fork the worker
				if(workers[j+(i*y)] < 0) //check if error
					perror("fork"); //if error, print error
				if(workers[j+(y*i)] == 0) { //check if it worked correctly
					float sum = 0; //set the sum to be 0
					int largest_value = 0; //set the largest to be 0
					for (int z = 0; z < students; z++) { //will go through each value in the column to calculate the averages and highest grades
						sum += grades[z][(y*i)+j]; //sums the grades up in each column
						if(grades[z][(y*i)+j] > largest_value) //checks for largest value
							largest_value = grades[z][(y*i)+j]; //sets largest value
						//printf("z=%d\n",z); //Uncoment to print the current number being checked
						//printf("grades[%d][%d] = %d\n",z,(y*i)+j,grades[z][(y*i)+j]); //uncomment to print the grades that are being checked.
					}
					max[j+(y*i)] = largest_value; //set max of the current worker to be the largest value
					averages[j+(y*i)] = sum/students; //set average of the current worker to be sum/studentsr
					printf("\nThe average of Chapter %d, homework %d, is %f\n",i+1,j+1,averages[j+(y*i)]); //print average for the chapter and hw
					printf("The highest grade of Chapter %d, homework %d, is %d\n",i+1,j+1,max[j+(y*i)]); //print max for the chapter and hw
					exit(0); //exits so it doesnt print multiple times
				}
				else {
					waitpid(workers[j+(i*y)],NULL,0); //waits for process to end
					//	printf("WORKER PID terminated : %d\n",waitpid(workers[j+(i*y)],NULL,0)); //uncomment to have PIDS printed out.
				}
			}
		exit(0); //exits so it doesnt print multiple times
		}
		else {
			waitpid(managers[i],NULL,0); //waits to process to end
		//	printf("MANAGER PID terminated : %d\n",waitpid(managers[i],NULL,0));	//uncomment to have PIDS printed out.		
		}
	}

return 0; //return
}
