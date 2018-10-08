#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 


void handle_sigint(int sig) 
{ 
    printf("\nCaught signal %d\n", sig); 
    exit(0);
} 
  
int main() 
{ 
    signal(SIGINT, handle_sigint); 
    while (1) 
    { 
        printf("hello world\n"); 
        sleep(1); 
    } 
    return 0; 
} 
  