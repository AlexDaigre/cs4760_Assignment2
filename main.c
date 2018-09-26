#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void abortExecution(int status);

int main (int argc, char *argv[]) {
    int numberOfChildren = 0;
    int maxProcesses = INT32_MAX;
    int currentProcesses = 0;
    int c;

    while ((c = getopt (argc, argv, "hn:s:")) != -1){
        switch (c){
            case 'h':
                printf("directions");
                abortExecution(0);
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
                abortExecution(1);
                break;
        }
    }

    for(int i = 0; i < numberOfChildren; i++){
        if (fork() == 0){
            execlp("./worker","./worker",numberOfChildren,NULL);
		    fprintf(stderr,"%s failed to exec worker!\n",argv[0]);
		    exit(-1);
            abortExecution(0);
        }
    }

    printf ("numberOfChildren = %d, maxProcesses = %d\n", numberOfChildren, maxProcesses);
    abortExecution(0);
}

void abortExecution(int status){
    exit(status);
}