#include<unistd.h>
#include <sys/uio.h>





int main(){

    int pid;

    pid = fork();

    if(pid){
        //parent
        struct iovec local[2];
        struct iovec remote[1];
        char buf1[10];
        char buf2[10];
        local[0].iov_base = buf1;
        local[0].iov_len = 10;
        local[1].iov_base = buf2;
        local[1].iov_len = 10;
        vector<unsigned long int> addrs = functionObjects[0].getaddresses();
        remote[0].iov_base = (void *)addrs[0] ;
        remote[0].iov_len = 20;

        process_vm_readv(pid,local,2,remote,1,0);

        cout << buf1 << " " << buf2 << endl;

    }
    else{
        //child



    }



}





