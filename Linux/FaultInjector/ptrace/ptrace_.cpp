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
#include <sys/personality.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>


struct addrRange {
        char StartAddr[16+1];
        char EndAddr[16+1];
        char perms[4];
        char path[126];
    }; 



std::vector<addrRange> ReadAddrs(int pid){

    // leggo proc/$pid/map e creo file 
    
    chdir("/");
    

    std::string commandStr = "cat proc/";
    std::string pid_s = std::to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps > /home/colo/FaultInjector/repo/Linux/FaultInjector/ptrace/addrs.txt");
    char command[commandStr.size()+1];
    strcpy(command,commandStr.c_str());
    system ((char *) &command);
    
    chdir("/home/colo/FaultInjector/repo/Linux/FaultInjector/ptrace"); // mi rimetto sulla directory corrente
    
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
            sAddr.append("0000"); // 4 zeri di offset per completare indirizzo (non sono sicuro)
           
            eAddr = line_s.substr(pos+1,pos);
            eAddr.append("0000");
          
            perms = line_s.substr(2*pos+2,4);
            
            pos = line_s.find_last_of("/");
            if(pos != std::string::npos){
                path = line_s.substr(pos);
            }else if((pos = line_s.find_first_of("[")) != std::string::npos){
                path = line_s.substr(pos);
            }

            //std::cout << "sAddr :" << sAddr << " eAddr :" << eAddr << " perms : "<< perms << " path : " << path << std::endl;
        
            

            strcpy(address.StartAddr,sAddr.c_str());
            strcpy(address.EndAddr,eAddr.c_str());
            strcpy(address.perms,perms.c_str());
            strcpy(address.path,path.c_str());

            
            //std::cout << "address.StartAddr :" << address.StartAddr << std::endl; 
            //std::cout << "atol : " << atoll((const char *)address.StartAddr) << std::endl;
            //std::cout << "address.EndAddr :" << address.EndAddr << std::endl; 
            //std::cout << "address.perms :" << address.perms << std::endl; 
            //std::cout << "address.path :" << address.path << std::endl; 
            

            addrsVec.push_back(address);
    }
    std::cout << "Address readed " << std::endl;
    return addrsVec;
};

void readP(std::vector<addrRange> address,int pid){


    return;

}
int main(){
            std::ofstream file;
            file.open("ip.txt");

            if(!file.is_open()){
                std::cout << "error open file " << std::endl;
            }
            
            auto pid = fork();
            if(pid < 0) {
                return -1;
            }

            if(pid == 0){
                // indirizzare stdout e stderr al file di uscita
                printf("my pid %d \n",getpid());
                //traceme
                printf("i'm the child \n");
                if(ptrace(PTRACE_TRACEME,0,nullptr,nullptr) < 0){
                    printf("ptrace error");
                    return 1;
                }
                personality(ADDR_COMPAT_LAYOUT);
                personality(ADDR_NO_RANDOMIZE);
                execl("Debugee2","Debugee2",nullptr);

            }else{

                int option = 0;
                int statusCode;
                int waitcode = waitpid(pid,&statusCode,option);

                if(waitcode == -1){
                    printf("error in waitpid");
                    return 0;
                }

                std::vector<addrRange> address = ReadAddrs(pid);
                //std::cout << "addr size :" << address.size() << std::endl;
                printf("Starting debugging loop \n");
                long count = 0;
                while(1){
                count++;
                if (WIFEXITED(statusCode)) {
                    printf("process exited,num istr %ld ,status=%d\n",count, WEXITSTATUS(statusCode));
                    break;

                } else if (WIFCONTINUED(statusCode)) {
                    printf("continued\n");
                } else if(WIFSTOPPED(statusCode)){
                    //printf("stopped \n");
                    // qui posso leggere
                    
                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS,pid,nullptr,&regs);

                //std::cout << "regs :"<< std::hex << regs.rip << std::endl;
                
                unsigned long long int rip = regs.rip;
                file << std::hex << rip;
                file << "\n";
                

                } else if (WIFSIGNALED(statusCode)){
                    printf("signaled");
                }

                if(ptrace(PTRACE_SINGLESTEP,pid,nullptr,nullptr) < 0){
                    perror("ptrace Single step");
                }
                
                //std::cout << "single step" << std::endl;

                /*
                if (ptrace(PTRACE_CONT, pid, nullptr, nullptr) < 0) {
                    perror("ptrace CONT ");                   
                  }
                */
                
                wait(&statusCode);
                }
               
            }
            file.close();
            printf("Fine \n");
            return 0;
}