#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main()
{	
	int fd; 
	fd = open("destination1.txt", O_RDWR|O_CREAT, 0777); //create a file if it doesnt exist, with read write and execute perms given by 0777
	if (fd == -1) //if there is an error, print it!
	{
		printf("\n open () failed with error \n");
	       	perror("open");

	}
	else
		printf("\n Successfully created or opened new file destination1.txt\n"); //confirms the creation of destination.txt
	
		
	int closefile = close(fd); //closes the file
	if (closefile == -1) //check if there were errors
	{
		printf("\n Error in closing the file\n"); //print an error msg
		perror("close"); //prints error
	}

	int fd2; 
	fd2 = open("destination2.txt", O_RDWR|O_CREAT, 0777); //create a file if it doesnt exist, with read write and executee perms given by 0777
	if (fd2 == -1) //checks if there was an error
	{
		printf("\n Open() failed with error \n"); //print an error msg
		perror("open"); //print error
	}
	else
		printf("\n Successfully created or opened new file destination2.txt\n"); //confirms creation of destination2.txt
	int closefile2 = close(fd2); //closes the file
	if (closefile2 == -1) //checks if error in closing it
	{
		printf("\n Error in closing the file\n"); //prints an error
		perror("close"); //prints error
	}
	return 0;
}
