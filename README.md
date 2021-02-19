# C/C++ fault injector

A software fault injector for C and C++ programs.


## Overview

The program follows the following steps:
- Extracts function names of the program, all the addresses of all the instructions and their length
- Runs a golden run without injection
- Runs the program N_injection_per_instruction (specified in `inputfile`) times for every instruction address of the program, each time with a flipped bit in at a random position.
- Creates a report of the injections



## Dependencies


- Dwarfdump
    

##  Installation



- Edit `inputfile`
    - First number: number of injections per address
    - Second number: multiplicator of the golden run execution time, to set the timeout on hanging runs
- Run `make injector`



## Usage
Compile your program with -g flag and place it into src/debugee/ then run   
`make objectfiles debugee="name of your file"`  

This will generate the files needed to extract the addresses to be injected and a directory in which the output file will be placed.


Run  
`./Injector "path of your file"`



## Output

Output is written in `src/output/name_of_your_program/injectorReport.csv` as csv rows.



| Index |Field |Meaning  |
| :------------- | :----------: | :----------: |
|0| Function/variable name  | Function or variable name the address belongs to  |
|1| Address  | Address that has been injected  |
|2| Injected bit | Nth bit injected. If set to `-1` the injection was not successful (ptrace(PEEKDATA,...) or ptrace(POKEDATA,...) failed).  |
|3| Run is correct  | ==1 : The run did not show any misbehavior.   |
|4| Output is different  | ==1:  The run's standard output is different from the golden run's one  |
|5|  Number of different chars | Number of different chars between golden and injected output, to quantify the difference between the two output  |
|6|  Timeout is expired | Child process has been terminated by timeout thread  |
|7| WIFEXITED(status) | Return value of the indicated macro (status is the status recieved from the last waitpid)  |
|8| WEXITSTATUS(status) | Return value of the indicated macro (status is the status recieved from the last waitpid)  |
|9| WIFSIGNALED(status)  |Return value of the indicated macro (status is the status recieved from the last waitpid)   |
|10|  WTERMSIG(status)  | Return value of the indicated macro (status is the status recieved from the last waitpid)  |
|11|  WIFSTOPPED(status)  | Return value of the indicated macro (status is the status recieved from the last waitpid)  |
|12|  WSTOPSIG(status)  | Return value of the indicated macro (status is the status recieved from the last waitpid)  |
|13|  WCOREDUMP(status)  | Return value of the indicated macro (status is the status recieved from the last waitpid)  |

For information about the status value: https://man7.org/linux/man-pages/man2/wait.2.html


## Notes and remarks

- Some error output from the injected program can be untraced because the error is sent to `/dev/tty`. one example of such error is `*** stack smashing detected ***: ./debugee/basicmath_small terminated`, that can be replicated by running the `basicmath_small` sample program.

- The main hardship in developing this project was that basically no oracle for testing exists, so we have no sample output to test our program against. This makes debugging and even understanding if the program is doing what is supposed to do very hard.

- By running the injector you are running a program with unpredictably modified instruction. There is no way to know what those function may cause to your machine. You may want to run this on a virtual machine. Be aware that we do not take any responsibility on the usage of our code.

- The behavior of the system call upon which this code is based on, `ptrace()`, seems to be working quite differently across systems. We tested the code on several machines and in some of them the system call simply didn't work as expected. We have no idea of how to fix this, for now.

## Documentation

- [Extractor](./Documentation/Extractor.md)
- [Debugger](./Documentation/Debugger.md)
- [BreakPoint](./Documentation/Breakpoint.md)
- [InjectionPoint](./Documentation/InjectionPoint.md)
- [FunctionObject](./Documentation/FunctionObject.md)
- [CompareFiles](./Documentation/CompareFiles.md)
- [InstructionObject](./Documentation/InstructionObject.md)






## Bibliography

-  We learnt about ptrace here:
https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/

- The samples programs found in ./debugeee have been taken from
http://vhosts.eecs.umich.edu/mibench/



