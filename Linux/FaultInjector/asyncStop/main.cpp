#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

using namespace std;

struct params {
    int pid; // pid of Deguee program
    int goldenExTime; // time golden ex
    int molt; // moltiplicator (input)
};


void * resetThread(void * p){
    cout << "[Thread] i'm the killer thread" << endl;
    struct params * para;
    para = (struct params *) p;
    int exit;
    sleep(para->goldenExTime * para->molt);

    if(kill(para->pid, SIGKILL) == 0){
        cout << "[Thread] now i kill the child process" << endl;
        exit = 1;
    }else{
        cout << "[Thread] child process is immortal" << endl;
        exit = -1;
    }

    pthread_exit((void *) exit);

}

int main(){

        int pid;
        int wait_status;
        int options = 0;
        int c = 0;

        while(c < 10){
        c++;
        
        cout << "[Main] iteration number " << c << endl;

        if(pid = fork()){
            
            struct params para;
            para.goldenExTime = 100;
            para.molt = 1;
            para.pid = pid;

            cout << "[Parent] starting killer thread" << endl;
            
            pthread_t t1;
            if(pthread_create(&t1,NULL,resetThread,(void *) &para ) != 0){
                cout << "[Parent] error creating the thread, exit.." << endl;
                return -1;
            }

            waitpid(pid, &wait_status, options);
            pthread_cancel(t1); // se il processo termina prima del contatore del thread da errore ma pacenza
            
            
            if(wait_status){
                cout << "[Parent] child process exit done code :" << wait_status << endl;
            }

            cout << "[Parent] exit, child code: "<< wait_status << endl;

        }else{
            cout << "[Child] i'm the Child" << endl;
            int *arg = (int *) 10;
            sleep(20);
            cout << "[Child] if you see this i had a good life" << endl;          
        }
        cout << endl;
        }
}

/*

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

*/