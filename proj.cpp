#include<iostream>
#include<stdio.h> 
#include <fstream>

using namespace std;

#define BUFSIZE 512


int main(){

    char buf[BUFSIZE];
    char bufchange[BUFSIZE];
    
    
    
    ofstream out("out",ios::out | ofstream::binary);
    ifstream myFile ("prog", ios::in | ios::binary);

    streambuf *coutbuf = std::cout.rdbuf();
    cout.rdbuf(out.rdbuf());




    while (myFile.read (buf, BUFSIZE)) {
        // Do whatever you want here...
        
        //cout << "First:  "<< buf << endl;
        //buf[0] = 'a';
        cout << buf  ;
    }







}

