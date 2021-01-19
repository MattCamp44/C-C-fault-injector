#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

int main(){

        int pid;

        if(pid = fork()){
            cout << "avvio thread per reset" << endl;

            std::thread t1([pid]()
            {
            sleep(20);
            int esit = kill(pid, SIGKILL);
            if(esit == 0){
                cout << "segnale inviato" << endl;
            }else{
                cout << "error kill" << endl;
            }


            }
            );
            t1.detach();
            
            auto resetThreadId = t1.get_id();

            int wait_status;
            auto options = 0;
            waitpid(pid, &wait_status, options);
            
            t1.
            
            if(wait_status){
                cout << "child process exit done code :" << wait_status << endl;
            }

            cout << "exit" << endl;
        }else{
            cout << "i'm the child" << endl;
            int *arg = (int *) 10;
            execl("WaitFor","WaitFor",arg,nullptr);          
        }
        


}