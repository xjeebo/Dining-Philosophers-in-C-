#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define PCNT 5			// the amount of philosophers that are dining
#define ROOMSIZE 4		// maximum amount of philosophers that can eat at a table
#define EATMAX 5		// max number of times a philosopher can eat
#define THINKING 1		// when the philosopher is done thinking and is now ready to eat	    
#define LFORK 2			// when the philosopher is taking the left fork
#define RFORK 3    	    // when the philosopher is taking the right fork
// Function Prototypes
void *algorithm(void *phil_id);
void *deadlock(void *phil_id);
//
//
//
//
//
//
//
//
// initialized as an array to implement left and right fork ex. philosopher 1 will have 
// have left fork (1) and right fork (2)
sem_t forks[5]; 		// five forks available to use on the table (shared resource)
sem_t room;     		// access to the room, which is ROOMSIZE (4)
// to keep track of how many times a philosopher eats which is 5
int EVERYBODYFULL = 0;
//int DEADLOCKFLAG = 0;
//
//
//
//
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
int main(int argc, char *argv[]){
//system("clear");
pthread_t pth; 
// for the forloop	
int i;		

// initialize the amount of forks to 5...for every philosopher there is 1 fork
for(i=0; i<PCNT; i++)
sem_init(&forks[i],1,1);


// if argv == 1 strcmp will output a 0, 0 signifies that it is equal to "1"
if(strcmp(argv[1], "1") == 0) {
	 	//DEADLOCKFLAG = 1;
		for(i=0; i<PCNT; i++) 	// to simulate 5 dining philosopher, we create 5 threads
        pthread_create(&pth,NULL,(void *)&deadlock,(void *)(intptr_t)i);  
}
else
	if(strcmp(argv[1], "0")==0){ // if strcmp() is = to "0" it will output a "0"
		//DEADLOCKFLAG = 0;
		// initialize room space to 4 t prevent deadlock
		sem_init(&room,1,ROOMSIZE);
		// flag variable to use sleep after left fork or not
		for(i=0; i<PCNT; i++) 	// to simulate 5 dining philosopher, we create 5 threads
		pthread_create(&pth, NULL,(void *) &algorithm, (void *)(intptr_t)i);
	}

// terminates calling thread to terminate further processing
pthread_exit(NULL);

return 0;
}
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
//
//
//
//
// Deadlock situation
// ..sleep(1); was implemented after the left fork was picked up
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
void *deadlock(void *phil_id){
// use the 	
int id = (intptr_t) phil_id;
int count = 0;
		// philosophers can only eat at most 5 times
        while(count < EATMAX){
        printf("Philosopher %d: is done thinking and now ready to eat.\n", id);
        

        // sem wait will remove a semaphor
        // a philosopher needs a left fork so one fork in the array will be removed
        // a philosopher has there own left fork that corresponds to there I.D.; (Phil 1 has left fork (fork 1 in the array of sem)
        sem_wait(&forks[id]);
        printf("Philosopher %d: taking left fork %d\n", id, id);
        // sleep is to display the deadlock situation where a fork is being shared
        // or picked up at once
        sleep(1);
        // the 4rth philosopher has fork 0 to his right because he is sitting next to philosopher 0
        // on his left is his own fork (fork 4) because every philosopher has at least one fork initially
        if(id == 4){
        	// philosopher 4 will take fork 0 that is on his right
            sem_wait(&forks[0]);
            printf("Philosopher %d: taking right fork 0\n", id);
        }
        
        else{
        	// the right fork of all philosophers except 4 is the philosophers ID+1
        	// this is how semaphors in the array are maintained 
            sem_wait(&forks[id+1]);
            printf("Philosopher %d: taking right fork %d\n", id, id+1);
        }
        
		  printf("Philosopher %d: EATING.\n", id);
	

		// put the left fork back down so it is available for use
		// the semaphor is put back into the array
        sem_post(&forks[id]);
        printf("Philosopher %d: putting down left fork %d\n", id, id);
        
        if(id == 4){
            sem_post(&forks[0]);
            printf("Philosopher %d: putting down right fork 0\n", id);
        }
        
        else{
		// put the right fork back down so it is available for use
		// the semaphor is put back into the array        	
            sem_post(&forks[id+1]);
            printf("Philosopher %d: putting down right fork %d\n", id, id+1);
        }
        
        // keep track how many times the philosopher in the particular thread had eaten
        count++;
        // everybodyfull variable will increment from all threads so if it 
        // equals 25.. it means that all 5 threads/philosophers ate
        EVERYBODYFULL++;
        if(EVERYBODYFULL == 25)
        printf("\nAll Philosophers have ate 5 times\n");
}
}//-----------------------------------------------------------------------
//
//
//
//
//
// 5 dining philosophers all getting to finish eating 5 times
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
//_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~_.-~
void *algorithm(void *phil_id){
// use the 	
int id = (intptr_t) phil_id;
int count = 0;
		// philosophers can only eat at most 5 times
        while(count < EATMAX){
         printf("Philosopher %d: is done thinking and now ready to eat.\n", id);
        
        // occupy a seat at the dining table
        sem_wait(&room);
        // a philosopher will take a fork from the array
        sem_wait(&forks[id]);
        printf("Philosopher %d: taking left fork %d\n", id, id);
        
        // every philosopher has there own left fork..the right fork is 
        // there id+1
        // the only one that has a special case is philosopher 4
        // who has a right fork of 0 and left fork of 4 because 
        // philosohper 4 sits next to 0
        if(id == 4){
        	// philosopher 4 occupys fork 0
            sem_wait(&forks[0]);
            printf("Philosopher %d: taking right fork 0\n", id);
        }
        
        else{
        	// the right fork  is the phlosophers id+1 which is equivalent to 
        	// the next philosophers left fork
            sem_wait(&forks[id+1]);
            printf("Philosopher %d: taking right fork %d\n", id, id+1);
        }
        
        printf("Philosopher %d: EATING.\n", id);
        
        // bring back the fork semaphor into the array of semaphors
        sem_post(&forks[id]);
        printf("Philosopher %d: putting down left fork %d\n", id, id);
        // philosopher 4 is a special case because he sits next to philosopher 0
        if(id == 4){
        	// philosopher 4 puts down philosopher 1's left fork
            sem_post(&forks[0]);
            printf("Philosopher %d: putting down right fork 0\n", id);
        }
        
        else{
        	// the philosopher puts down his right fork which is the next philosophers
        	// left fork
            sem_post(&forks[id+1]);
            printf("Philosopher %d: putting down right fork %d\n", id, id+1);
        }
        // the philosopher has finished eating this is to ensure all philosophers get 
        // a chance to sit at the dining table
        sem_post(&room);
        
        count++;
        // everybodyfull variable will increment from all threads so if it 
        // equals 25.. it means that all 5 threads/philosophers ate
        EVERYBODYFULL++;
        if(EVERYBODYFULL == 25)
        printf("\nAll Philosophers have ate 5 times\n");
}
}//-----------------------------------------------------------------------
