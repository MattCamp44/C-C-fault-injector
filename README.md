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
make objectfiles debugee="name of your file"

Run
./Injector "name of your file"



## Output
The output is expressed through a .csv file. Each line represents a run in which one address has been injected. Each row has the following form:
| Function/variable name       | Address     | Column 3     |
| :------------- | :----------: | -----------: |
|  Cell Contents | More Stuff   | And Again    |
| You Can Also   | Put Pipes In | Like this \| |


