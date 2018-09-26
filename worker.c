#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void abortExecution(int status);

int main (int argc, char *argv[]) {
    int numberOfloops = atoi(argv[1]);

    for(int i = 0; i < numberOfloops; i++){
        printf("Hi");
    }
    
    abortExecution(0);
}

void abortExecution(int status){
    exit(status);
}