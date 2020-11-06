#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>


#define MAX_PATH 256 // path al file map table nel range della map table
#define PERM 5 // numero caratteri per rappresentare permessi
class Debugger{
// classe che fa da debugger e injetta gli errori
private:
    struct addrRange {
        unsigned long * StartAddr;
        unsigned long * EndAddr;
        char * perms;
        char * path;
    }; 
    
    int pid;
    char * progName;
public:
Debugger(){
    this->pid = 0;
    progName = nullptr;
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
        std::vector<addrRange> address = ReadAddrs(pid);
        inject(address);
        int istr = 0;
        
        while(1){
                        
            istr++;
            if(ptrace(PTRACE_SINGLESTEP,pid,nullptr,nullptr)){
                perror("ptrace");
                break;
            }
            //if(istr % 1000 == 0  ) readRegs(istr); 1/1000 delle volte
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
void readRegs(int istr){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS,pid,0,&regs);
    unsigned int peeked_istr = ptrace(PTRACE_PEEKDATA,pid,(void *)regs.rip,0);
    printf("istruction [%d] \n RIP : 0x%08llx \n istruction : 0x%08x \n rax : %08llx \n",istr,regs.rip,peeked_istr,regs.rax);
    return;
};
std::vector<addrRange> ReadAddrs(int pid){

    // leggo proc/$pid/map e creo file 
    
    chdir("/");
    

    std::string commandStr = "cat proc/";
    std::string pid_s = std::to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps > /home/colo/FaultInjector/repo/Linux/FaultInjector/addrs.txt");
    char command[commandStr.size()+1];
    strcpy(command,commandStr.c_str());
    system ((char *) &command);
    
    chdir("/home/colo/FaultInjector/repo/Linux/FaultInjector/"); // mi rimetto sulla directory corrente
    
    // read file
    
    std::ifstream addrsFile("addrs.txt");
    char * line = nullptr;
    std::string line_s;
    FILE * fd = fopen("addrs.txt","r");
    int n;
    size_t size;
    int pos;
    int pos1;
    unsigned char addr[12];
    std::string sAddr;
    std::string eAddr;
    std::string perms;
    std::string path;
    std::vector<addrRange> addrsVec;
    addrRange address;

    while((n = getline(&line,&size,fd) != -1)){
        
            line_s = static_cast<std::string>(line);
            pos = line_s.find_first_of('-');
            // 12 caratteri per l'indirizzo
            
            sAddr = line_s.substr(0,pos);
            eAddr = line_s.substr(pos+1,pos);
            perms = line_s.substr(2*pos+2,4);
            
            pos = line_s.find_last_of("/");
            if(pos != std::string::npos){
                path = line_s.substr(pos);
            }else if((pos = line_s.find_first_of("[")) != std::string::npos){
                path = line_s.substr(pos);
            }

            //std::cout << "sAddr :" << sAddr << " eAddr :" << eAddr << " perms :"<< perms << " path :" << path << std::endl;
            
            address.StartAddr = ( unsigned long *) sAddr.c_str();
            address.EndAddr =   ( unsigned long *) eAddr.c_str();
            address.perms = (char *) perms.c_str();
            address.path = (char *) path.c_str();

            
            std::cout << "address.StartAddr :" << address.StartAddr << std::endl; 
            /*
            std::cout << "address.EndAddr :" << address.EndAddr << std::endl; 
            std::cout << "address.perm :" << address.perms << std::endl; 
            */
            std::cout << "address.path :" << address.path << std::endl; 
            

            addrsVec.emplace_back(address);

    
    }
    std::cout << "Address readed" << std::endl;
    return addrsVec;
};
void inject(std::vector<addrRange> addrs){

    // (forse non necessario) cambio permessi accesso alle pagine tramite mprotect
    int i = 0;
    addrRange ad ;
    while(i <= addrs.size()){ // trovo il primo range con path = /Debugee1
        ad = addrs[i];
        std ::cout << "qui" << std::endl;
        if(strcmp((char *) "/Debugee1",ad.path) == 0){
            break;
        }
        i++;
    }
    
    unsigned long * addrMain = ad.StartAddr;
    unsigned long  offsetMain = 0x1169;
    unsigned long finalAddr = (*addrMain + offsetMain); // segmentation fault ??
    
    std ::cout << "addrMain :" << addrMain << " offsetMain :" << offsetMain << "final address : " << finalAddr << std::endl;

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
