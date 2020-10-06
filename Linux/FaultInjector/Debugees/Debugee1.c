#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    printf("Debugee program waiting \n");
    sleep(10);
    printf("Debugee program exit \n");
    return 1;
}