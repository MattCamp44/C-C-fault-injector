#include "stdio.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char ** argv){

    int pid;
    

    for(int i = 0; i < argc ; i++)
        printf("%s\n",argv[i]);

    char * newargv[20];

    for(int a = 0; a < 20; a++)
        newargv[a] = malloc( 100*sizeof(char) );

    for(int j = 2; j < argc; j++)
        strcpy(newargv[j-2],argv[j]);
    newargv[argc-1] =NULL;
    printf("\nArgv:\n");
    printf("%s\n",argv[1]);
    for(int b = 0; b < argc - 2 ; b++)
        printf("%s\n",newargv[b]);


    printf("Before execv\n");
    int exevfailed;
    exevfailed = execv(argv[1],newargv);

    printf("%d\nErrno=%d\n",exevfailed,errno);

    printf("After execv??\n");
}











