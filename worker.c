#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

// void abortExecution(int status);

int main (int argc, char *argv[]) {
    int numberOfloops = atoi(argv[1]);
    int clockShmId = atoi(argv[2]);

    int* clockShmPtr = (int *) shmat(clockShmId, NULL, 0);
    if ((int) clockShmPtr == -1) {
        printf("shmat error in child\n");
        exit(1);
    }
    
    int i;
    for(i = 0; i < numberOfloops; i++){
        clockShmPtr[1]++;
        if (clockShmPtr[1] > 1000){
            clockShmPtr[0]++;
            clockShmPtr[1] -= 1000;
        }
        // printf("S:%d MS:%d\n", clockShmPtr[0], clockShmPtr[1]);
    }
    
    shmdt(clockShmPtr);
    exit(0);
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