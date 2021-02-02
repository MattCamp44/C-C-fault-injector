# Cyringe 

Cyringe is a software fault injector for C and C++ programs.

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
| Function/variable name       | Address     | N bit injected |Run is correct     | Output is different     | Number of different lines   | Specified timeout |Timeout  | Error |
| :------------- | :----------: | -----------: | :------------- | :----------: | -----------: |-----------: |-----------: |-----------: |
|  Function or variable name the address belongs to  | Address that has been injected   | Nth bit injected  | The run did not show any misbehavior compared to the golden run. If its value is 1 the next fields are irrelevant and set to 0  | The run's standard output is different from the golden run's one   | Number of different lines between golden and injected output | User Specified timeout   |The specified timeout has expired  | The program has written something on stderr    |
|


