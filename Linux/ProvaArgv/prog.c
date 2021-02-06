#include "stdio.h"

int main(int argc, char ** argv){


    printf("Im the second program\n");
    for(int i = 0; i < argc ; i++)
        printf("Secondprogram %s\n",argv[i]);

}