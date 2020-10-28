#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    int sec = 50;
    printf("Process start with pid : %d \n",getpid());
    printf("Process will wait for %d sec \n",sec);
    sleep(sec);
    printf("Bye Bye \n");
    return 0;
}