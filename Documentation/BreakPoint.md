# BreakPoint

A BreakPoint is a special instruction that can create an interrupt and stop the execution of the process.
Basically it consist placing the instruction `0xCC` (`int3`) in the corresponding address at the beginning of the original instruction.
When the breakpoint is reached the corresponding instruction, replaced with `int3`, is restored and the execution resumed.
The class is able to place breakpoints everywhere in the code using ptrace and specifing the address where to put breakpoint.
Breakpoint contains the function to enable breakpoint and release it when the execution reach the breakpoint.


Functions:

|Function Name|Desctiption |
| :------|:--:|
|Enable| Place the `int3` istruction in the corresponding address and save the original istructon|
|Release|Replace the `int3` instruction with the original one|

