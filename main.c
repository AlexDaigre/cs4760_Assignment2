#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

void abortExecution(int status);
void childClosed(int sig);
int currentProcesses = 0;
//acctually about 3 seconds(on my system). CLOCKS_PER_SECOND is a constant and 
//  does not produce anything resembling real time.
int maxTime = 20;

int main (int argc, char *argv[]) {
    signal(SIGCHLD, childClosed);

    int numberOfChildren = 0;
    int maxProcesses = 20;
    int c;

    while ((c = getopt (argc, argv, "hn:s:")) != -1){
        switch (c){
            case 'h':
                printf("directions\n");
                exit(0);
                break;
            case 'n':
                numberOfChildren = atoi(optarg);
                break;
            case 's':
                maxProcesses = (atoi(optarg) > 20) ? 20 : atoi(optarg);
                break;
            // case '?':
            // case ':':
            default:
                printf("there was an error with arguments\n");
                exit(1);
                break;
        }
    }
    printf ("numberOfChildren = %d, maxProcesses = %d\n", numberOfChildren, maxProcesses);
    printf ("calculating time...\n");
    

    int clockShmId = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
    if (clockShmId < 0) {
        printf("shmget error in parrent\n");
        exit(1);
    }

    int* clockShmPtr = (int *) shmat(clockShmId, NULL, 0);
    if ((long) clockShmPtr == -1) {
        printf("shmat error in parrent\n");
        shmctl(clockShmId, IPC_RMID, NULL);
        exit(1);
    }
    
    int numberOfRepetitions = numberOfChildren * 1000000;
    clock_t startTime = clock();
    clock_t timeDiffrence;
    int breakLoop = 0;
    int i;

    do{
        for(i = 0; i < numberOfChildren; i++){
            while (currentProcesses >= maxProcesses ){sleep(1);}
            currentProcesses++;
            if (fork() == 0){
                char numberOfRepetitionsString[12];
                sprintf(numberOfRepetitionsString, "%d", numberOfRepetitions);
                char clockShmIdString[12];
                sprintf(clockShmIdString, "%d", clockShmId);

                execlp("./worker","./worker", numberOfRepetitionsString, clockShmIdString, NULL);
                fprintf(stderr,"%s failed to exec worker!\n",argv[0]);
                shmctl(clockShmId, IPC_RMID, NULL);
                shmdt(clockShmPtr);
                exit(1);
            }
            timeDiffrence = clock() - startTime;
            printf("number of processes: %d\ncurrent time elapsed: %ld\n", currentProcesses, (timeDiffrence * 1000 / CLOCKS_PER_SEC));
        }
        breakLoop = (i < numberOfChildren) ? 0 : 1;
    }while (((timeDiffrence * 1000 / CLOCKS_PER_SEC) < maxTime) && (breakLoop == 0));
   

    // while (currentProcesses > 0 ){sleep(1);}

    printf("Main: S:%d MS:%d\n", clockShmPtr[0], clockShmPtr[1]);
    shmctl(clockShmId, IPC_RMID, NULL);
    shmdt(clockShmPtr);
    exit(0);
}


void childClosed(int sig){
    currentProcesses--;
    printf("Child Closed\n");
}

// void abortExecution(int status){
//     exit(status);
// }

// void abortExecution(int status, int clockShmId){
//     shmctl(clockShmId, IPC_RMID, NULL);
//     exit(status);
// }

// void abortExecution(int status, int clockShmId, int clockShmPtr){
//     shmctl(clockShmId, IPC_RMID, NULL);
//     shmdt(clockShmPtr);
//     exit(status);
// }