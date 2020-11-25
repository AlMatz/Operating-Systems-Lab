#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"
#include <sys/sem.h>

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0
#define FALSE      			1

int smoker_match, lock, smoker_paper, smoker_tobacco, agent; //semaphores for all counters and expressors
int Number_of_Smokes, Latch;

main() {
    smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT); //express value of entering CS for smoker_match
    lock = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for the lock
    smoker_paper = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for smoker_paper
    smoker_tobacco = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering cs for smoker_tobacco
    agent = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //express value of entering CS for agent
    Number_of_Smokes = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //counter for how many smokes will be done
    Latch = semget(IPC_PRIVATE,1,0666|IPC_CREAT); //For agent to run others
    if (agent == -1) {
        printf("error in semaphore creation\n");
        exit(1);
    }

    int pid;						// Process ID after fork call
    int status;						// Exit status of child process
    int N = 5;                      // Number of times to smoke

    sem_create(smoker_match,0); //create semaphore with smoker_match
    sem_create(lock,1); //create semaphore with lock
    sem_create(smoker_paper,0); //create semaphore with smoker_paper
    sem_create(smoker_tobacco,0); //create semaphore with smoker_tobacco
    sem_create(agent,0); //create semaphore with agent
    sem_create(Number_of_Smokes,N); //create semaphore with Number_of_Smokes as the amount of times we will smoke
    sem_create(Latch,1); //Acts as flag for executing.

    if ((pid = fork()) == -1)
    {
        //fork failed!
        perror("fork");
        exit(1);
    }

    if (pid == CHILD) {
        for(int i=1; i<=N; i++) {
            //printf("int i = %d \n",i);
                P(lock);
                int randNum = rand() % 3 + 1; // Pick a random number from 1-3
                if (randNum == 1) {
                    // Put tobacco on table
                    // Put paper on table
                    printf("Agent puts tobacco and paper on table \n");
                    V(smoker_match);  // Wake up smoker with match
                } else if (randNum == 2) {
                    // Put tobacco on table
                    // Put match on table
                    printf("Agent puts tobacco and match on table \n");
                    V(smoker_paper);  // Wake up smoker with paper
                } else {
                    // Put match on table
                    // Put paper on table
                    printf("Agent puts match and paper on table \n");
                    V(smoker_tobacco);
                } // Wake up smoker with tobacco
                V(lock);
                P(agent);  //  Agent sleeps
                P(Number_of_Smokes);
            }  // end forever loop
            P(Latch);
        V(smoker_paper);
        V(smoker_tobacco);
        V(smoker_match);
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
        if (pid == CHILD) {
            while (semctl(Number_of_Smokes,0,GETVAL)) {
                P(smoker_tobacco);  // Sleep right away
                P(lock);
                if(semctl(Latch,0,GETVAL)) {
                    // Pick up match
                    // Pick up paper
                    printf("smoker_tobacco Picks up match and paper\n");
                }
                V(agent);
                V(lock);
                // Smoke (but don't inhale).
            }
        }
        else {
            //Parent Process. Fork off another child process.
            if ((pid = fork()) == -1)
            {
                //Fork failed!
                perror("fork");
                exit(1);
            }
            if (pid == CHILD) {
                while (semctl(Number_of_Smokes,0,GETVAL)) {
                    P(smoker_match);  // Sleep right away
                    P(lock);

                    if(semctl(Latch,0,GETVAL)) {
                        // Pick up tobacco
                        // Pick up paper
                        printf("smoker_match Picks up tobacco and paper\n");
                    }
                    V(agent);
                    V(lock);
                    // Smoke (but don't inhale).
                }

            }
            else {
                //Parent Process. Fork off another child process.
                if ((pid = fork()) == -1)
                {
                    //Fork failed!
                    perror("fork");
                    exit(1);
                }
                if (pid == CHILD) {
                    while (semctl(Number_of_Smokes,0,GETVAL)) {
                        P(smoker_paper);  // Sleep right away
                        P(lock);

                        if(semctl(Latch,0,GETVAL)) {
                            // Pick up match
                            // Pick up tobacco
                            printf("smoker_paper Picks up match and tobacco\n");
                        }
                        V(agent);
                        V(lock);
                        // Smoke (but don't inhale).
                    }
                }
                else
                {
                    //Now wait for the child processes to finish
                    pid = wait(&status);
                    printf("Process(pid = %d) exited with the status %d. \n", pid, status);

                    pid = wait(&status);
                    printf("Process(pid = %d) exited with the status %d. \n", pid, status);

                    pid = wait(&status);
                    printf("Process(pid = %d) exited with the status %d. \n", pid, status);

                    pid = wait(&status);
                    printf("Process(pid = %d) exited with the status %d. \n", pid, status);



                }
                exit(0);
            }
            exit(0);
        }
       exit(0);
    }
    exit(0);
    semkill(smoker_match);
    semkill(smoker_tobacco);
    semkill(smoker_paper);
    semkill(agent);
    semkill(lock);
}
