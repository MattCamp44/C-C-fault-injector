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
#include <string.h>

void procmsg(const char* format, ...)
{
    va_list ap;
    fprintf(stdout, "[%d] ", getpid());
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}

void run_target(const char* programname)
{
    procmsg("target started. will run '%s'\n", programname);

    /* Allow tracing of this process */
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");
        return;
    }

    /* Replace this process's image with the given program */
    execl(programname, programname, (char *)NULL);
}

void run_debugger(pid_t child_pid)
{
        
    int wait_status;
    struct user_regs_struct regs;

    procmsg("debugger started\n");

    /* Wait for child to stop on its first instruction */
    wait(&wait_status);

    /* Obtain and show child's instruction pointer */
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    procmsg("Child started. RIP = 0x%016x\n", regs.rip);

    
    long long int addr = 0x0000000000001169;
    printf("Indirizzo BP : ");
    scanf("%lld \n",&addr);
    printf("the addr is : %lld",addr);
    unsigned long long int data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr,NULL);
    procmsg("Original data at 0x%08x: 0x%08x\n", addr, data);

    //test
    unsigned long long int data_u = data;
    procmsg("test data_u: 0x%08x\n", data_u);

    /* Write the trap instruction 'int 3' into the address */
    unsigned long long int data_with_trap = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
    procmsg("Data with trap : 0x%016llx \n",data_with_trap);
    if(ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)data) == -1){
        printf("error : %s \n",strerror(errno));
    }

    /* See what's there again... */
    unsigned long long int readback_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, 0);
    if(readback_data == -1){
        printf("error : %s \n",strerror(errno));
    }
    procmsg("After trap,set breakpoint, data at 0x%016llx: 0x%016llx\n", addr, readback_data);

    /* Let the child run to the breakpoint and wait for it to
    ** reach it
    */
    ptrace(PTRACE_CONT, child_pid, 0, 0);

    //wait to breakpoint
    wait(&wait_status);
    if (WIFSTOPPED(wait_status)) {
        procmsg("Child got a signal: %s\n", strsignal(WSTOPSIG(wait_status)));
    }
    else {
        perror("wait");
        return;
    }

    /* See where the child is now */
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    procmsg("Child stopped at RIP = 0x%016x\n", regs.rip);

    /* Remove the breakpoint by restoring the previous data
    ** at the target address, and unwind the EIP back by 1 to
    ** let the CPU execute the original instruction that was
    ** there.
    */
    int result = ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)data);
    procmsg("poketext origin  back result %d\n", result);
/* See what's there again... */
     readback_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, 0);
     procmsg("After restore, data at 0x%llx: 0x%llx\n", addr, readback_data);
     int offset = 0;
     for(offset = 1; offset < 25; offset++) {
       readback_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)(addr+offset), 0);
       procmsg("After restore, data at 0x%llx: 0x%llx\n", addr+offset, readback_data);
     }




    regs.rip -= 1;
    ptrace(PTRACE_SETREGS, child_pid, 0, &regs);

    /* The child can continue running now */
    ptrace(PTRACE_CONT, child_pid, 0, 0);

    wait(&wait_status);
    if (WIFEXITED(wait_status)) {
        procmsg("Child exited\n");
    } else if(WIFSIGNALED(wait_status)) {
    procmsg("signal !!!\n");
    }
    else {
        procmsg("Unexpected signal. %s \n",  strsignal(WSTOPSIG(wait_status)));
    }
 } 

int main(int argc, char** argv)
{
    pid_t child_pid;

    if (argc < 2) {
        fprintf(stderr, "Expected a program name as argument\n");
        return -1;
    }

    child_pid = fork();
    if (child_pid == 0)
        run_target(argv[1]);
    else if (child_pid > 0){
        printf("PID : %d \n",child_pid);
        run_debugger(child_pid);
    } else {
        perror("fork");
        return -1;
    }

    return 0;
}