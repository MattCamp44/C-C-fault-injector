# C/C++ fault injector

A software fault injector for C and C++ programs.


## Overview


## Dependencies

- Boost
    https://www.boost.org/doc/libs/1_75_0/more/getting_started/index.html
- Dwarfdump
    

##  Installation
Download the repository, go into src/ directory and use
make main (???)

## Usage
Compile your executable with -g flag and place it into src/debugee/ then run   
`make objectfiles debugee="name of your file"`

Run  
`./Injector "name of your file"`



## Output
The output is expressed through a .csv file. Each line represents a run in which one address has been injected. Each row has the following form:
| Function/variable name       | Address     | N bit injected |Run is correct     | Output is different     | Number of different chars   | Specified timeout |Timeout  | Error |
| :------------- | :----------: | -----------: | :------------- | :----------: | -----------: |-----------: |-----------: |-----------: |
|  Function or variable name the address belongs to  | Address that has been injected   | Nth bit injected  | The run did not show any misbehavior compared to the golden run. If its value is 1 the next fields are irrelevant and set to 0  | The run's standard output is different from the golden run's one   | Number of different chars between golden and injected output, to quantify the difference between the two output | User Specified timeout   |The specified timeout has expired  | The program has written something on stderr    |



## Notes and remarks

- Some error output from the injected program con be untraced because the error is sent to `/dev/tty`. one example of such error is `*** stack smashing detected ***: ./debugee/basicmath_small terminated`, that cna be replicated by running the `basicmath_small` sample program.

- The main hardship in developing this project was that basically no oracle for testing exists, so we have no sample output to test our program against. This makes debugging and even understanding that the program is doing what is supposed to very hard

- By running the injector you are running a program with unpredictably modified instruction. There is no way to know what those function may do to your machine. You may want to run this on a virtual machine. Be warned, we do not take any responsibility on the usage of our code.



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

-  The core functionality of this project is a readaptation of this tutorial:
https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/

- The samples programs found in ./debugeee have been take from
http://vhosts.eecs.umich.edu/mibench/



