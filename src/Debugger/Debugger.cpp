#include "./Debugger.h"
#include "sys/ptrace.h"
#include "sys/wait.h"
#include<vector>
#include "../FunctionObject/FunctionObject.h"
#include "../BreakPoint/BreakPoint.h"
#include "../InjectionPoint/InjectionPoint.h"
#include <assert.h> 
#include<sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include<stdio.h>
#include "../Output_functions/comparefiles.h"
#include<fstream>
#include "../InstructionObject/InstructionObject.h"
#include <algorithm>


using namespace std;

void continue_execution(int pid) {
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(pid, &wait_status, options);

}


struct params {
    int pid; // pid of Deguee program
    double goldenExTime; // time golden ex
    int molt; // moltiplicator (input)
};


void * resetThread(void * p){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    // cout << "[Thread] i'm the killer thread" << endl;
    struct params * para;
    para = (struct params *) p;
    int exit;   



    // The multiplicator, even at ridiculously high numbers, keeps on killing the child process -> +1
    // printf("Sleeping for %d\n",(int)(para->goldenExTime * para->molt)+1 );
    // sleep((long)(para->goldenExTime* 1000 * para->molt)+1);
    sleep(2);
    // sleep(1);
    // usleep(1000);
    // cout << "thread woke up\n";

    if(kill(para->pid, SIGKILL) == 0){
        printf("[Thread] now i kill the child process\n");
        // cout << "[Thread] now i kill the child process" << endl;
        exit = 1;
    }else{
        // cout << "[Thread] child process is immortal" << endl;
        exit = -1;
    }

    pthread_exit((void *) &exit);

}





int EnableInjectionPoint(int pid, InstructionObject address ){

    

    
    InjectionPoint injPoint = InjectionPoint(pid,address.getAddress(),address.getLength());

    
    return injPoint.InjectFirstBit();


}



void Debugger(vector<FunctionObject> FunctionObjects, char * prog, int NinjectionsPerAddress,double goldenExecutionTime,char ** newargv){



    int totalinstructions = 0;

    for(auto FunctionObject : FunctionObjects)
        for(auto i : FunctionObject.getaddresses()){
            for( auto j = 0; j < NinjectionsPerAddress; j++ ){
                totalinstructions++;
            }}

    
    fstream outputFile;
    string outputFilePath = "./output/";
    string prognamestring = prog;
    string progNameWithoutPath = prognamestring.substr(prognamestring.find_last_of("/\\") + 1);
    outputFilePath.append(progNameWithoutPath);
    outputFilePath.append("/injectorReport.csv");
    cout << outputFilePath << endl;

    outputFile.open(outputFilePath,ios::out | ios::app); 
     



    int pid;
    
    vector<int> pids;
    int progress = 0;
    int index=0;
    for(auto FunctionObject : FunctionObjects)
    for(auto i : FunctionObject.getaddresses()){
        for( auto j = 0; j < NinjectionsPerAddress; j++ ){
    
        
        pid = fork();
        if(pid){
            //Parent

            progress++;
            pids.emplace_back(pid);
            waitpid(pid,nullptr,0);
            

            
            
            
            unsigned long int breakpointAddress = i.getAddress();



            int InjectedBit = EnableInjectionPoint(pid,i);



            BreakPoint breakpoint(pid,breakpointAddress);
            




            breakpoint.Enable();

            // From here the child process continues with the breakpoint inserted
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);


            // Wait for the child process to arrive at the breakpoint
            waitpid(pid,nullptr,0);
            
            

            user_regs_struct regs;


            breakpoint.Release();

            //Get current registers
            ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

            //reset program counter
            regs.rip = breakpointAddress ;
            ptrace(PTRACE_SETREGS, pid, nullptr, &regs);


            //The child process proceeds with the injected instruction    
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //To get from main..
            int maxTimerMultiplicator = 4;



            // Thread that handles the timeout
            struct params para;
            para.goldenExTime = goldenExecutionTime;
            para.molt = 3;
            para.pid = pid;

            pthread_t t1;
            
            if(pthread_create(&t1,NULL,resetThread,(void *) &para ) != 0){
                cout << "[Parent] error creating the thread, exit.." << endl;
                return ; //return error code
            }

            

            int status;
            

            // Wait for the child process to terminate
            int waitPidReturn = waitpid(pid,&status,0);



            



            // A way to detect a stack smashing (since it is not directed to stderr). The values have been derived
            // "empirically", we don't know if this works for similar errors or what these values mean
            // if(WIFEXITED(status) == 0 && WEXITSTATUS(status)==6 && WIFSTOPPED(status)==1 && WSTOPSIG(status)==6)
            //     errorGenerated = 1;


            //Even better than above: if the process did not terminate normally something must have happened => error 
            // (we don't even have to check the stderr?)
           
                

                

            int timeoutExpired = 0;


            //WTERMSIG(status) == 9 => the process has been killed => timeout expired
            if(WIFSIGNALED(status) && WTERMSIG(status) == 9){
                timeoutExpired=1;
                //The child process has been killed -> The thread has terminated
                pthread_join(t1,NULL);
            } 



            

            //Cancel the timeout thread
            pthread_cancel(t1); 

            

            
           

            
            // Compare the stdout of the golden run and the injected run -> return number of different chars
            int comparefiles = compareFiles();



            // The commented section below was supposed to detect an error by checking if
            // stderr of the child process (redirected to injectedoutputstderr.txt) is 
            // empty or not. We used another strategy but this may come handy

            // ifstream ifile;
            // ifile.open("injectedoutputstderr.txt");
            // // If file exists and it is not empty -> error
            // if(ifile && ifile.peek() != std::ifstream::traits_type::eof()) {
            //     cout << ifile.peek() ;
            //     errorGenerated = 1;
            // } else {
            //     // cout << ifile.peek() ;
            //     // cout<<"No errors\n";
            // }
            // ifile.close();
            // remove("injectedoutputstderr.txt");
            

            remove("injectedoutput.txt");
            // cout << "Done injecting\n";
            int runiscorrect = (!comparefiles && !timeoutExpired) ? 1 : 0;

            // This is really brute forcing killing the last 10 preocesses for every run
            // But it's the only way we've found not to have plenty of zombie processes
            int waitpidreturn;
           
            int beginpoint = pids.size() < 10 ? 0 : pids.size() - 10;
            int endpoint = pids.size();

            for(auto p = beginpoint; p < endpoint ; p++){
                // cout << "Killing process " << p << endl;
                kill(pids[p],SIGKILL);
                waitpidreturn = waitpid(pids[p],NULL,WNOHANG);
                // cout << waitpidreturn << endl;
                // if(waitpidreturn == -1)
                //     pids.erase(remove(pids.begin(),pids.end(),p),pids.end());
            }

            // kill(pid,SIGKILL);
            // waitpid(pid,NULL,WNOHANG);
            // if(compareFiles == 0)
            outputFile << FunctionObject.getname() << "," << hex << i.getAddress() << "," << dec <<  InjectedBit << "," << runiscorrect << "," << (comparefiles != 0 ) << ","  << comparefiles  << "," << timeoutExpired  <<  "," << WIFEXITED(status) << "," << WEXITSTATUS(status) << "," << WIFSIGNALED(status) << "," << WTERMSIG(status) << "," << WIFSTOPPED(status) << "," << WSTOPSIG(status) << "," << WCOREDUMP(status) <<  endl ;
            // else cout << compareFiles << endl;
            printf("%d of %d injections...      \r",progress,totalinstructions); 
             

        }

        else{
            //child
            freopen("injectedoutput.txt", "w", stdout);
            freopen("injectedoutputstderr.txt", "a", stderr);
            ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
            execv(prog,newargv);
            cout << "Execv failed\n";
            exit(1);
        }
    }

} //for

    return;

}



















