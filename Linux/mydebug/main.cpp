#include<sys/ptrace.h>
#include<iostream>
#include <sys/types.h>
#include <unistd.h>
#include<string>
//#include "./debugger/debugger.hpp"
#include<vector>
#include <sys/wait.h>
#include<sstream>


class breakpoint{
    public:
        breakpoint(pid_t pid, std::intptr_t addr): m_pid{pid}, m_enabled{false}, m_saved_data{}
        {}

        void enable(){
            
            auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
            m_saved_data = static_cast<uint8_t>(data & 0xff); // save bottom byte
            uint64_t int3 = 0xcc;
            uint64_t data_with_int3 = ((data & ~0xff) | int3); // set bottom byte to 0xcc
            ptrace(PTRACE_POKEDATA, m_pid, m_addr, data_with_int3);

            m_enabled = true;
            
            };
        void disable(){
            
            auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
            auto restored_data = ((data & ~0xff) | m_saved_data);
            ptrace(PTRACE_POKEDATA, m_pid, m_addr, restored_data);
            
            
            };

        auto is_enabled() const -> bool {return m_enabled; }
        auto get_address() const -> std::intptr_t {return m_addr; }
    
    private:
        pid_t m_pid;
        std::intptr_t m_addr;
        bool m_enabled;
        uint8_t m_saved_data;
    
    
    };




class debugger{

    private:

    std::string progname;
    pid_t pid;


    public:

    debugger(std::string prog, pid_t pid):
    progname{std::move(prog)} , pid{pid}{}




    void run(){
        
        int wait_status;
        auto options = 0;
        waitpid(this->pid,&wait_status, options);

        std::string line;

        while((std::getline(std::cin,line))){
            
            if(!line.compare("stop")) break; 

            //handle_command(line);
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);
            int wait_status;
            auto options =0;
            waitpid(pid,&wait_status, options);


            }


        }
    
    
    
    };


// file lista di guasti -> TEMPO E LUOGO
// ENG non finisce mai, output sbagliato, output corretto, crash
// tempo esecuzione per ENG
// tutta memoria, dati/istruzioni, iniettare solo main/funzione, stack/heap
// qt
// Scegliere 
// segnalazione ad eventi
// bus library 

int main(int argc, char* argv[]){
    
    auto prog = argv[1];

    auto pid = fork();
    
    if(pid == 0){
        //child
        
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        
        execl(prog,prog,nullptr);
        
        }

    else{
        //parent
        
        debugger dbg(prog, pid);
        
        dbg.run();
        
        
        }


    
    
    }
