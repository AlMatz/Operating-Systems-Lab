#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main() 
{
	char* sourcefile = "source.txt"; //the location of the source file 
	char* destinationfile = "destination1.txt"; //the location of the destination file
	char* seconddestinationfile = "destination2.txt"; //location of the 2nd destination file.
		errno = 0;
		int fd; //for the source file
		int fd2; //for destination file
		int fd3; //for destination2 file 
		fd = open(sourcefile,O_RDONLY); //checks read permission for source file
		if (fd == -1) //if we dont have permission it will print an error below
		{
			printf("\n Source File open() failed with error\n"); //prints error
			perror("open"); //print the error
			return 1; //returns
		}
		fd2 = open(destinationfile,O_RDWR); //cheks for read and write permission for destination file
		if (fd2 == -1) //if we dont have permission it will print error below
		{
			printf("\n Destination File open() failed with error\n"); //prints error
			perror("open"); //print the error
			return 1; //returns 
		}

		fd3 = open(seconddestinationfile, O_RDWR); //opens second destination file to write to
		if (fd3 == -1) //checks if error in opening
		{
			printf("\n Destination2 File Open() failed with error\n"); //if there was an error print a msg
			perror("open"); //print error
			return 1; //return
		}
		
		//now use the same technique as before to read from the old file and write to the new file
		char buff[1]; //buffer to hold what we read.
		int count_bytes = 0; //to count 50 bytes.
		int mux = 0; //to determine which destination we are writing to, 1 for destination 2, 0 for destination 1.
		while(read(fd,buff,1)!=0) //a while to keep reading till we reach the end of the file
		{
			
			if (count_bytes < 50 && mux == 0) //if we read under 50 bytes and we are in mux = 0
			{
				count_bytes++; //increase the amount of bytes read
				if (buff[0] == '5' || buff[0] == '8') //check if the current thing read is a 5 or an 8
					write(fd2,buff,1); //if it is, write it to dest1 folder.
			}
			
			else if(count_bytes == 50 && mux == 0) //if we get to 50 bytes and we are at mux 0
			{
				mux = 1; //set mux to 1 
				count_bytes = 1; //inc bytes to 1 bc we want to reset the count but we are still writing another byte
				write(fd3,buff,1); //write the char to dest 2.
			}

			else if(count_bytes < 100 && mux == 1) //if we read under 100 bytes and are in mux 1
			{
				count_bytes++; //inc bytes read 
				write(fd3,buff,1); //write to destination 2
			}

			else if(count_bytes == 100 && mux == 1) //if we read 100 bytes, and mux is 1
			{
				mux = 0; //set mux to 0
				count_bytes = 1; //set count_bytes to 1 bc we want to reset count but we are still writing another byte
				if (buff[0] == '5' || buff[0] == '8') //if it is a 5, or an 8, we only want to write those
					write(fd2,buff,1); //write to destination 1 file
			}
			
		}
		//closes the source file and checks if there is an error in closing it. Prints the error if there is.
		int closefile1 = close(fd);
		if (closefile1 == -1) {
			printf("\n Error in closing source file\n");
			perror("close");
		}

		//closes the first destination file and checks if there is an error in closing it. Prints the error if there is.
		int closefile2 = close(fd2);
		if (closefile2 == -1) {
			printf("\n Error in closing destination file\n");
			perror("close");
		}

		//closes the second destination file and checks if there is an error in closing it. Prints the error if there is.
		int closefile3 = close(fd3);
		if (closefile3 == -1) {
			printf("\n Error in closing destination2 file\n");
			perror("close");
		}


return 0;
}
