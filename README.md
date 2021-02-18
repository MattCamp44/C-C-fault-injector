# C/C++ fault injector

A software fault injector for C and C++ programs.


## Overview


## Dependencies


- Dwarfdump
    

##  Installation
Download the repository, go into src/ directory and use
make main (???)

## Usage
Compile your executable with -g flag and place it into src/debugee/ then run   
`make objectfiles debugee="name of your file"`  

This will generate the files needed to extract the addresses to be injected and a directory in which the output file will be placed.


Run  
`./Injector "path of your file"`



## Output
The output is expressed through a .csv file. Each line represents a run in which one address has been injected. Each row has the following form:
| Function/variable name       | Address     | Injected bit |Run is correct     | Output is different     | Number of different chars   | Specified timeout |Timeout  | Error |
| :------------- | :----------: | -----------: | :------------- | :----------: | -----------: |-----------: |-----------: |-----------: |
|  Function or variable name the address belongs to  | Address that has been injected   | Nth bit injected  | The run did not show any misbehavior compared to the golden run. If its value is 1 the next fields are irrelevant and set to 0  | The run's standard output is different from the golden run's one   | Number of different chars between golden and injected output, to quantify the difference between the two output | User Specified timeout   |The specified timeout has expired  | The program has written something on stderr    |



| Index |Field |Meaning  |
| :------------- | :----------: | :----------: |
|0| Function/variable name  | Function or variable name the address belongs to  |
|1| Address  | Address that has been injected  |
|2| Injected bit | Nth bit injected. If set to `-1` the injection was not successful (ptrace(PEEKDATA,...) or ptrace(POKEDATA,...) failed).  |
|3| Run is correct  | ==1 : The run did not show any misbehavior.   |
|4| Output is different  | ==1:  The run's standard output is different from the golden run's one  |
|5|  Number of different chars | Number of different chars between golden and injected output, to quantify the difference between the two output  |
|6|  Specified timeout | User Specified timeout  |
|7|  |  |

## Notes and remarks

- Some error output from the injected program can be untraced because the error is sent to `/dev/tty`. one example of such error is `*** stack smashing detected ***: ./debugee/basicmath_small terminated`, that can be replicated by running the `basicmath_small` sample program.

- The main hardship in developing this project was that basically no oracle for testing exists, so we have no sample output to test our program against. This makes debugging and even understanding if the program is doing what is supposed to do very hard.

- By running the injector you are running a program with unpredictably modified instruction. There is no way to know what those function may cause to your machine. You may want to run this on a virtual machine. Be aware that we do not take any responsibility on the usage of our code.

- The behavior of the system call upon which this code is based on, `ptrace()`, seems to be working quite differently across systems. We tested the code on several machines and in some of them the system call simply didn't work as expected. We have no idea of how to fix this, for now.

## Documentation

- [Extractor](./Documentation/Extractor.md)
- [Debugger](./Documentation/Debugger.md)
- [AddressSelector](./Documentation/AddressSelector.md)
- [BreakPoint](./Documentation/Breakpoint.md)
- [InjectionPoint](./Documentation/InjectionPoint.md)
- [FunctionObject](./Documentation/FunctionObject.md)
- [CompareFiles](./Documentation/CompareFiles.md)
- [Debugee](./Documentation/Debugee.md)






## Bibliography

-  We learnt about ptrace here:
https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/

- The samples programs found in ./debugeee have been take from
http://vhosts.eecs.umich.edu/mibench/



