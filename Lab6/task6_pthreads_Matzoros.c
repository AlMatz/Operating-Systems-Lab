#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>

//Mutex locks for threads
pthread_mutex_t agent_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smokermatch_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smokerpaper_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smokertobacco_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int N = 6; //Number of times to smoke
int flag = 1; //Flag for printing critical section

void *agent_process() {
    while(N) {
        pthread_mutex_lock(&lock);
        //pthread_mutex_lock(&agent_lock);
        printf("iteration N = %d\n", N);
        int randNum = rand() % 3 + 1; // Pick a random number from 1-3
        //pthread_mutex_lock( &agent_lock);
        if (randNum == 1) {
            // Put tobacco on table
            // Put paper on table
            printf("Agent puts tobacco and paper on table \n");
            pthread_mutex_unlock(&smokermatch_lock); //unlock smoker match

        } else if (randNum == 2) {
            // Put tobacco on table
            // Put match on table
            printf("Agent puts tobacco and match on table \n");
            pthread_mutex_unlock(&smokerpaper_lock); //unlock smoker paper

        } else {
            // Put match on table
            // Put paper on table
            printf("Agent puts match and paper on table \n");
            pthread_mutex_unlock(&smokertobacco_lock); //unlock smoker tobacco
        }
        pthread_mutex_lock(&agent_lock);
        pthread_mutex_unlock(&lock);
        N--;
        sleep(1);
    } // end loop
    //run them all once more so that they can end
    flag = 0; //set flag to 0 so critical section doesn't run
    pthread_mutex_unlock(&smokermatch_lock);
    pthread_mutex_unlock(&smokerpaper_lock);
    pthread_mutex_unlock(&smokertobacco_lock);

}

void *smoker_match() {
    while(N) {
        pthread_mutex_lock(&smokermatch_lock); //lock it right away
        pthread_mutex_lock(&lock); //lock the lock
        if (flag) //CS
        printf("smoker_match Picks up paper and tobacco \n");
        pthread_mutex_unlock(&lock); //unlock the lock
        pthread_mutex_unlock(&agent_lock); //unlock the agent;
    }
}

void *smoker_paper() {
    while(N) {
        pthread_mutex_lock(&smokerpaper_lock); //lock it right away
        pthread_mutex_lock(&lock); //lock the lock
        if (flag) //CS
        printf("smoker_paper Picks up tobacco and match \n");
        pthread_mutex_unlock(&lock); //unlock the lock
        pthread_mutex_unlock(&agent_lock); //unlock the agent;
    }
}

void *smoker_tobacco() {
    while(N) {
        pthread_mutex_lock(&smokertobacco_lock); //lock it right away
        pthread_mutex_lock(&lock); //lock the lock
        if(flag) //CS
        printf("smoker_tobacco Picks up paper and match \n");
        pthread_mutex_unlock(&lock); //unlock the lock
        pthread_mutex_unlock(&agent_lock); //unlock the agent;
    }
}

void main() {

    //Thread IDs used to create thread
    pthread_t agent;
    pthread_t smokermatch;
    pthread_t smokertobacco;
    pthread_t smokerpaper;

    //pthread_mutex_lock(&agent_lock);
    pthread_mutex_lock(&smokertobacco_lock);
    pthread_mutex_lock(&smokerpaper_lock );
    pthread_mutex_lock(&smokermatch_lock );

    //Create the pthreads for each of the ID's
    pthread_create(&agent, NULL, agent_process, NULL);
    pthread_create(&smokermatch, NULL, smoker_match, NULL);
    pthread_create(&smokertobacco, NULL, smoker_tobacco, NULL);
    pthread_create(&smokerpaper, NULL, smoker_paper, NULL);

    //Wait for each thread to end
    pthread_join(agent,NULL);
    pthread_join(smokermatch,NULL);
    pthread_join(smokertobacco,NULL);
    pthread_join(smokerpaper,NULL);

}