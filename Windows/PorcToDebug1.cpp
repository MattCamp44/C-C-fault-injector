
#include <Windows.h>
#include <iostream>

using namespace std;

int main(){

		cout << "i'm a process and i will count from 0 to 100" << endl;
		for(int i=0;i<100;i++){
			cout << i << endl;
			Sleep(100);
		}
	
}