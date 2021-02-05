# Debugger



## Timeout Thread
After the child process is let continue, a thread is started with function `resetThread`. The thread sleeps for `para->goldenExTime* 1000 * para->molt)+1`, after that the child process is killed.  
As for specifications, the thread should only wait `para->goldenExTime* 1000 * para->molt`, that is the golden run time multiplied by the user specified multiplicator. We added the `+1` at the end because we noted, by testing with different values of `para->molt`, that the outcome would be that the thread kills every process for timeout.  
We consider this as an effect of the process creation and system call overhead that inevitably pollute the runtime measurement of the child process.