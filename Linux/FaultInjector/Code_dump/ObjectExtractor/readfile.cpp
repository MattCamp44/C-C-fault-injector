#include<iostream>
#include<string>
#include<fstream>

using namespace std;

int main(){
    
    
    fstream newfile;

    newfile.open("file",ios::in);

    string buf;

    while(getline(newfile,buf))
        cout << buf << endl;



    
    
    
    
    
    }




