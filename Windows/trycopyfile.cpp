#include <fstream>
#include<iostream>
#define N 512

using namespace std;

int main()
{   
    char buffer[N];
    std::ifstream  src("prog", std::ios::binary);
    std::ofstream  dst("out",   std::ios::binary);
    
    while(true){
        src.rdbuf() -> pubsetbuf(buffer, N);
        cout << buffer << endl;
        cout << src.rdbuf();
    }

}