#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void function1(){
    printf("this is function1 \n");
    int n = 0;
    while(n<10){
        n++;
    }
    return;
}

void function2(){
    printf("this is function2 \n");
    return;
}

int main(){
    printf("Debugee program start \n");
    function1();
    function2();
    sleep(10);
    printf("Debugee program exit \n");
    return 1;
}