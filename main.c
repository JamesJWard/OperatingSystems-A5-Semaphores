#include "header.h"

sem_t * semScrn;
sem_t * semKey;

/*Creates named semaphores, then forks 4 processes that call runProcess, and finally waits
for the children processes to die before unlinking the semaphores and exiting.*/
int main(int argc, char const *argv[])
{
	pid_t pid[4];
	int i = 0;

	/*Open the semaphore used by processes*/
	sem_unlink(SCRNSEM);//unlink if already there
	sem_unlink(KEYBSEM);
	if((semScrn = sem_open(SCRNSEM,O_CREAT,S_IRGRP|S_IWGRP,1)) == SEM_FAILED){
		perror("sem_open1");
		exit(0);
	}
	if((semKey = sem_open(KEYBSEM,O_CREAT,S_IRGRP|S_IWGRP,1)) == SEM_FAILED){
		perror("sem_open2");
		exit(0);
	}
	
	/*fork children processes with indices*/
	for (i = 0; i < 4; i++){
		pid[i] = fork();
		if (pid[i] == 0){//child process, send off

			runProcess(i+1);
		}
	}
	
	//parent, wait for children
	if(pid != 0){
		//fprintf(stderr, "I am the parent with pid: %d\n", getpid());
		for(i=0;i<4;i++){
			if (waitpid(pid[i], NULL, 0) == -1){
				perror("waitpid");
			}
		}
		//once all children die, unlink semaphores
		sem_unlink(SCRNSEM);
		sem_unlink(KEYBSEM);
	}

	return 0;
}

/*Uses getSems() to lock the required semaphores, and then echos input until 
a 'q' is read. The function exits at the end (kills child once it has echoed 
enough times rather than returning to the fork loop). Before it exits, it 
posts and closes used semaphores.*/
void runProcess(int idx){

	srand(time(NULL));
	int a = 0;
	int dls = 0;
	char buffer[MAXBUFFER];
		
	for (a = 0; a < 2; a++){
		dls = getSems(idx);

		fprintf(stdout, ">> This is process %d: Had %d deadlocks\n", getpid(), dls);
		fprintf(stdout, ">> Enter a string or q to quit\n");
	
		do{
			fprintf(stdout, ">> ");
			fgets(buffer, MAXBUFFER, stdin);
			fprintf(stdout, ">> Echo: %s", buffer);
		}while(strncmp(buffer,"q",1) != 0);
	
		if (sem_post(semScrn) == -1 || sem_post(semKey) == -1){
			perror("sme_post");
		}
	}

	if (sem_close(semScrn) == -1 || sem_close(semKey) == -1){
		perror("sme_close");
	}
	exit(0);
}

/*locks the two semaphores (screen and keybrd) with deadlock checking and resolution,
 and returns the number of deadlock incountered.*/
int getSems(int idx){

	int deadlocks = 0;
	int gotBoth = 0;
	int r = 0;

	struct timespec ts;

	while(gotBoth == 0){

		if(clock_gettime(CLOCK_REALTIME, &ts) == -1){
			perror("clock_gettime");
		}
		ts.tv_sec += 1;

		if(idx%2==0){//even
			sem_wait(semKey);
			sleep(1);
			r = sem_timedwait(semScrn, &ts);
		}else{//odd
			sem_wait(semScrn);
			sleep(1);
			r = sem_timedwait(semKey, &ts);
		}

		if(r == -1){
			//fprintf(stderr, "got a deadlock!\n");
			if (errno == ETIMEDOUT){
				//deadlock, give back and wait rand time
				if(idx%2==0){
					if(sem_post(semKey)==-1){
						perror("sem_post");
					}
				}else{
					if(sem_post(semScrn)==-1){
						perror("sem_post");
					}
				}
			}else
				perror("sem_timedwait");

			deadlocks++;
			sleep(rand()%3);
		}else
			gotBoth = 1;
	}
	return deadlocks;
}