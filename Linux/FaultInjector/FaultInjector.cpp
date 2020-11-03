#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <string>
#include <string.h>
#include <fstream>

class Debugger{
// classe che fa da debugger e injetta gli errori
private:
    int pid;
    char * progName;
    int64_t WhereAddress = 0 ; // punto nel main

public:
Debugger(){
    this->pid = 0;
    progName = nullptr;
    WhereAddress = 0x000000000000117d ; // punto nel main
};
void start(){
    fprintf(stdout,"Debugger start \n");
    int pid = 0;
    pid = fork();
    if(pid == 0){
        printf("child \n");
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl("Debugee1","Debugee1",nullptr);

    }else{
        int status;
        printf("father \n");
        
        wait(&status);
        int istr = 0;
        ReadAddrs(pid); // read the address range of the process at creation time
        while(1){
                        
            istr++;
            if(ptrace(PTRACE_SINGLESTEP,pid,nullptr,nullptr)){
                perror("ptrace");
                break;
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("process exited \n");
                break;
            }
        }
        printf("tot istructions %d\n",istr);
    }

    return;
};
void ReadAddrs(int pid){

    // leggo proc/$pid/map e creo file 
    
    chdir("/");
    

    std::string commandStr = "cat proc/";
    std::string pid_s = std::to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps > /home/colo/FaultInjector/repo/Linux/FaultInjector/map_table.txt");
    char command[commandStr.size()+1];
    strcpy(command,commandStr.c_str());
    if(system ((char *) &command) == -1){
        printf("Error in system() %d \n",errno);
        return;
    }
    
    chdir("/home/colo/FaultInjector/repo/Linux/FaultInjector/Prove"); // mi rimetto sulla directory corrente
    
    printf("Addresses readed \n");

    return;
};
};

class Controller{
    // prende file eseguibile
// fa objdump per indirizzi
// crea pipe per raccogliere output
// comunica al Debugger dove metterre i BP e dove eseguire gli injecto e quanti
// prende output dal debugger 
// rilancia il debugger il numero di volte necessario
private :
    //Debugger dbg;
    char * progName;

public:
Controller(){
    //dbg = Debugger();
    progName = (char *) "Debugee1";
};
void start(){
    fprintf(stdout,"Controller start \n");
    
    Debugger dbg;

    dbg.start();
    
    return;
};
};

int main(){



    printf("Start \n");

    Controller controller;

    controller.start();

    printf("Stop \n");
}
