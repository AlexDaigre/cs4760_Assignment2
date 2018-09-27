#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

void abortExecution(int status);

int main (int argc, char *argv[]) {
    int numberOfloops = atoi(argv[1]);
    printf("number of loops: %d\n", numberOfloops);
    int sharedMemoryIntOne = atoi(argv[2]);
    printf("SharedMem1: %d\n", sharedMemoryIntOne);
    int sharedMemoryIntTwo = atoi(argv[3]);
    printf("SharedMem2: %d\n", sharedMemoryIntTwo);
    
    for(int i = 0; i < numberOfloops; i++){
        printf("Hi %d %d\n", sharedMemoryIntOne, sharedMemoryIntTwo);
    }
    
    abortExecution(0);
}

void abortExecution(int status){
    exit(status);
}