// prende file eseguibile
// fa objdump per indirizzi
// crea pipe per raccogliere output
// comunica al Debugger dove metterre i BP e dove eseguire gli injecto e quanti
// prende output dal debugger 
// rilancia il debugger il numero di volte necessario
#include "Debugger.h"
#include "Controller.h"
#include <iostream>

Controller::Controller(){};

void Controller::start(char * name){
            printf("controller start\n");
            Debugger dbg = Debugger();
            dbg.start(name);
};

