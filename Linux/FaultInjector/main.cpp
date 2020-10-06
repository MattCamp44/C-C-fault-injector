#include "Controller.h"

int main(){

    //controllo se mi ha passato il file
    //controller.start(file.json)

    Controller ctrl = Controller();
    ctrl.start((char*) "./Debugee/Debugee1");
}