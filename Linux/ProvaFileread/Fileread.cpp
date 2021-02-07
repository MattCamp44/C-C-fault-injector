#include<iostream>
#include <fstream>

using namespace std;

int main(){

    fstream newfile;
    newfile.open("inputfile",ios::in);

    if (newfile.is_open()){   //checking whether the file is open
      string tp;
      while(getline(newfile, tp)){  //read data from file object and put it into string.
         cout << stoi(tp) << "\n";   //print the data of the string
      }
      newfile.close(); 

    }








}