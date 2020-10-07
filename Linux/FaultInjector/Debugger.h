#include <unistd.h>
#include <stdlib.h>

class Debugger{
    private:
        int pid;
    public:
    int start(char * progName);
    Debugger();
};
