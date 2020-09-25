#include <Windows.h>
#include <iostream>

using namespace std;

int main() {

	DWORD millisecond = 60000;

	cout << "i'm the process to debug and i'm waiting" << endl;
	cout << "my Pid :" << GetCurrentProcessId() << endl;
	
	for (int i = 0; i <= 5; i++) {
		Sleep(millisecond);
		cout << 5 - i << "minute left" << endl;
	}
	
	
	cout << "done";



	return 0;
}