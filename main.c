#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

void abortExecution(int status);
void childClosed();
int currentProcesses = 0;

int main (int argc, char *argv[]) {
    int numberOfChildren = 0;
    int maxProcesses = __INT32_MAX__;
    // int maxProcesses = 1;
    int c;

    signal(SIGCHLD, childClosed);

    while ((c = getopt (argc, argv, "hn:s:")) != -1){
        switch (c){
            case 'h':
                printf("directions");
                exit(0);
                break;
            case 'n':
                numberOfChildren = atoi(optarg);
                break;
            case 's':
                maxProcesses = atoi(optarg);
            case '?':
            case ':':
            default:
                printf("there was an error with arguments");
                exit(1);
                break;
        }
    }
    printf ("numberOfChildren = %d, maxProcesses = %d\n", numberOfChildren, maxProcesses);
    printf ("calculating time...");
    

    int clockShmId = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
    if (clockShmId < 0) {
        printf("shmget error in parrent\n");
        exit(1);
    }

    int* clockShmPtr = (int *) shmat(clockShmId, NULL, 0);
    if ((int) clockShmPtr == -1) {
        printf("shmat error in parrent\n");
        shmctl(clockShmId, IPC_RMID, NULL);
        exit(1);
    }
    
    int numberOfRepetitions = numberOfChildren * 1000000;

    for(int i = 0; i < numberOfChildren; i++){
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
    }

    // while (currentProcesses > 0 ){sleep(1);}

    printf("Main: S:%d MS:%d\n", clockShmPtr[0], clockShmPtr[1]);
    shmctl(clockShmId, IPC_RMID, NULL);
    shmdt(clockShmPtr);
    exit(0);
}


void childClosed(){
    currentProcesses--;
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