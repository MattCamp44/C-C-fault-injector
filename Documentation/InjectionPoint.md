# InjectionPoint

This class aim is to flip a random bit of the instruction.
After the process trace, if the variable "data" is set to -1 the injection was not successful.
Then a casual number less than instruction's lenght is chosen, the bitwise operation XOR is performed between instruction and 1 in Nth position.
This operations combined together perform the bit flip in the Nth position.

functions:
|Function Name|Description|
|:----|:---:|
|InjectFirstBit|The injection of Nth bit is performed|






