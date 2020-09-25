/*
Versone 1.0.0 fa vedere soltanto le eccezioni che genera
*/


#include <iostream>
#include <Windows.h>
#include <windows.h>
#include <debugapi.h>
#include <process.h>
#include <processthreadsapi.h>
#include <string>
#include <tchar.h>
#include <WinBase.h>
#include <map>
using namespace std;



DWORD handleDebugEvent(LPDEBUG_EVENT debug_event);


int main() {
	cout << "[Main] Start" << endl;
	//creare processo con flag giuste
	STARTUPINFO infoStart;
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoStart, sizeof(infoStart)); // fill-0 della porzione di memoria indicata (?)
	infoStart.cb = sizeof(infoStart); // grandezza della struttura in byte
	ZeroMemory(&infoProcess, sizeof(infoProcess)); //  x2 (?)

	LPTSTR ProcToDebug = (LPTSTR)_T("ProcToDebug.exe");

	if (!CreateProcess(ProcToDebug, NULL, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &infoStart, &infoProcess)) {
		cout << "[Main] fail creating process...exit" << endl;
		exit(0);
	}

	cout << "[Main] Process to debug created " << endl;

	//debugger loop
	DEBUG_EVENT debug_event = { 0 };
	DWORD dwContinueStatus;

	cout << "[Main] enter debugger loop" << endl;

	while (1) {

		// inizio ciclo aspettando un debug event
		
		if (!WaitForDebugEvent(&debug_event, INFINITE)) {
			cout << "[Main] error wait debug event" << endl;
			exit(1);
		}
		
		
		//cout << "[Main] on the debug loop.." << endl;
		Sleep(1000);
		
		// qui mettere comandi e chiamata alle funzioni corrispondenti e lo switch case in per il tipo di evento generato
		// il primo evento coincide con la creazione del processo
		dwContinueStatus = handleDebugEvent(&debug_event);

		//cout << "[Main] now restart process to debug" << endl;
		// procedo con il prossimo debug event
		if (!ContinueDebugEvent(debug_event.dwProcessId,debug_event.dwThreadId, dwContinueStatus = DBG_CONTINUE)) { 
			cout << "[Main] error continue debug event" << endl;
			exit(1);
		}

	}
	cout << "[Main] exit" << endl;
	return 1;
}

DWORD handleDebugEvent(LPDEBUG_EVENT debug_event) {

	// qui la logica del fault injector
	DWORD contStatus = DBG_EXCEPTION_NOT_HANDLED; // significa eccezione non gestita, se invece si gestisce l'eccezione usare DBG_CONTINUE
	//cout << "[EventHandler] event code :" << debug_event->dwDebugEventCode << endl;
	switch (debug_event->dwDebugEventCode) {

			case CREATE_PROCESS_DEBUG_EVENT:{
				cout << "[EventHandler] CREATE_PROCESS_DEBUG_EVENT" << endl;
				break;
				};
			case CREATE_THREAD_DEBUG_EVENT: {
				cout << "[EventHandler] CREATE_THREAD_DEBUG_EVENT" << endl;
				break;
			}
			case EXCEPTION_DEBUG_EVENT: {
				cout << "[EventHandler] EXCEPTION_DEBUG_EVENT ->";
				// scrivere tipo di eccezzione  per capire che eccezioni crea il programma scritto
				EXCEPTION_DEBUG_INFO& Exception = debug_event->u.Exception;
		
				switch (Exception.ExceptionRecord.ExceptionCode) {
					case EXCEPTION_BREAKPOINT: {
						cout << "EXCEPTION_BREAKPOINT " << endl ;
						cout << "	information" << Exception.ExceptionRecord.ExceptionInformation << endl;
						cout << "insert " << endl;
						char in;
						cin >> in;
						break;
				}
					case EXCEPTION_SINGLE_STEP: {
						cout << "EXCEPTION_SINGLE_STEP " << endl;
						cout << "	information" << Exception.ExceptionRecord.ExceptionInformation << endl;
						cout << EXCEPTION_ACCESS_VIOLATION << endl;
						break;
					}
					default: {
						cout << "other exception" << endl;
						break;
					}
				}

				break;
			}
			case RIP_EVENT: {
				cout << "[EventHandler] RIP EVENT the process has terminated apparently for no reason" << endl;
				break;
			}
			case EXIT_PROCESS_DEBUG_EVENT: {
				cout << "[EventHandler] EXIT_PROCESS_DEBUG_EVENT" << endl;
				break;
			}
		
			case EXIT_THREAD_DEBUG_EVENT: {
				cout << "[EventHandler] EXIT_THREAD_DEBUG_EVENT" << endl;
				break;
			}
			case LOAD_DLL_DEBUG_EVENT:{
				cout << "[EventHandler] LOAD_DLL_DEBUG_EVEN" << endl;
				break; 
			}
			case UNLOAD_DLL_DEBUG_EVENT: {
				cout << "[EventHandler] UNLOAD_DLL_DEBUG_EVENT" << endl;
				break; 
			}
			case OUTPUT_DEBUG_STRING_EVENT:{
				cout << "[EventHandler] OUTPUT_DEBUG_STRING_EVENT" << endl;
				break; 
			}
	
	}

	return contStatus;

}