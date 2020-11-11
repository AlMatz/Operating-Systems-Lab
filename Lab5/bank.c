#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"
#include <sys/sem.h>

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */
int sem,sem1,sem2,sem3,sem4,sem5,sem6; //semaphores for all counters and expressors

main()
{
	sem = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //give sem the value of semget
	if (sem == -1) {
		printf("error in semaphore creation\n");
		exit(1);
	}
	sem1 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for Dad
	sem2 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for son 1
	sem3 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for son 2
	sem4 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //Count time for Dad
	sem5 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //Count time for son 1
	sem6 = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //Count time for son 2

	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables
	sem_create(sem,1); //create semaphore with sem 
	sem_create(sem1,1); //create semaphoes with the sem values
	sem_create(sem2,1);
	sem_create(sem3,1);
	sem_create(sem4,1);
	sem_create(sem5,1);
	sem_create(sem6,1);		
			
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);
	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=9;
		for(i=1;i<=N; i++)
		{
			V(sem1); //increment Dad bc it expressed interest
			P(sem); // <=== Inserted P(sem)
			
			//check if two children are trying to access it if either one is, increase count for it
			if(semctl(sem2,0,GETVAL) == 1)
				V(sem5);
			if(semctl(sem3,0,GETVAL) == 1)
				V(sem6);

			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
			
			V(sem); // <=== Inserted V(sem)
			P(sem1); //decrement dad interest done
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{	
				V(sem2); //increment child 1
				P(sem); // <== Inserted P(sem)

				if(semctl(sem1,0,GETVAL) == 1) //check if dad is wanting to access, if it is increment it
					V(sem4);
				
				if(semctl(sem3,0,GETVAL) == 1) //check if son 2 is wanting to access it, if it increment it
					V(sem6);

				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}
				P(sem2);
				V(sem); //<=== Insert V(sem)
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					V(sem3); //increment access for 2nd child
					P(sem); //<=== Inserted P(sem) 

					if(semctl(sem1,0,GETVAL) == 1) //check if dad is waiting, if it is increment time
						V(sem4);

					if(semctl(sem2,0,GETVAL) == 1) //check if sone 1 is waiting, if it is increment time
						V(sem5);

					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}
					V(sem); // <= Inserted V(sem)
					P(sem3); //decrement access for 2nd child
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);


				printf("Dad waiting time is  %d\n",semctl(sem4,0,GETVAL));
				printf("Son 1 waiting time is %d\n",semctl(sem5,0,GETVAL));
				printf("Son 2 waiting time is %d\n",semctl(sem6,0,GETVAL));
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}
