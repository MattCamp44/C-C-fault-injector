#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc,char * argv[]){

    if(argc < 2){
        printf("insert time in second \n");
        return -1;
    }
    
    int time = atoi(argv[1]);
    printf("My pid is %d \n",getpid());
    printf("i will wait for %d seconds\n",time);
    sleep(time);
    printf("exit \n");
    return 1;
}