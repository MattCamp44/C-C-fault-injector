#include<unistd.h>
#include <sys/uio.h>
#include<sys/ptrace.h>
#include<sys/wait.h>
#include<iostream>

using namespace std;


int main(){

    int pid;

    pid = fork();

    if(pid){
       //parent
       
       auto data = ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr);
       

       cout << "Parent" << endl;

       waitpid(pid,NULL,0);

    }
    else{
        //child
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        sleep(1);
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;
        cout << "Child" << endl;

    }



}





