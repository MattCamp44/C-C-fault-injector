#include "Controller.h"
#include <iostream>
int main(){

    //controllo se mi ha passato il file
    //controller.start(file.json)
    printf("start \n");
    
    Controller ctrl = Controller();
    ctrl.start((char*) "./Debugee/Debugee1");

    printf("stop \n");
}