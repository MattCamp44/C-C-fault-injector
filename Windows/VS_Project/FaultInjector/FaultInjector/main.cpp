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
#include <dbghelp.h> // simboli non trovati
#include <psapi.h>
#include <strsafe.h>

#define BUFSIZE 512

using namespace std;

TCHAR dllName[MAX_PATH + 1];

BOOL GetFileNameFromHandle(HANDLE hFile) // trovata fatta 
{
	BOOL bSuccess = FALSE;
	TCHAR pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		_tprintf(TEXT("Cannot map a file with a length of zero.\n"));
		return FALSE;
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);

	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{

				// Translate path with device name to drive letters.
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
									&& *(pszFilename + uNameLen) == _T('\\');

								if (bFound)
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									StringCchPrintf(szTempFile,
										MAX_PATH,
										TEXT("%s%s"),
										szDrive,
										pszFilename + uNameLen);
									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}
	//_tprintf(TEXT("File name is %s\n"), pszFilename);
	wcscpy_s(dllName, pszFilename);
	return(bSuccess);
}

DWORD handleDebugEvent(LPDEBUG_EVENT debug_event);

//DWORD processInfo(HANDLE hProcess); // vedere sotto, in teoria l'inizializzazione del caricamento va tutta prima del loop
// mentre per quanto riguarda il caricamento dei simboli si utilizza SymLoadModule64 e si prende il nome della DLL dal file handler
// bisogna guardare la documentazione per capire come, una volta caricati i simboli posso settare i breakpoint

HANDLE hProcess,hThread;
BOOL tfSetted = FALSE;

void start() {
	//scritta iniziale
cout << " _______ _ ______  _ "<< endl;
cout << "(_______|_|______)| |" << endl;
cout << " _____   _ _     _| |__   ____" << endl;
cout << "|  ___) | | |   | |  _ \ / _  |" << endl;
cout << "| |     | | |__/ /| |_) | (_| |" << endl;
cout << "|_|     |_|_____/ |____/ \___ |" << endl;
cout << "                        (_____|" << endl;

}

int main(DWORD argc,LPTSTR argv[]) {
	cout << " Fault Injector Start " << endl;

	start();
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa); // per far ereditare l'handle del file, forse gli devo passare un estremo della pipe
	// questo non funziona forse perche' gli passo un file gia' aperto
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	STARTUPINFO infoStart;
	PROCESS_INFORMATION infoProcess;
	
	HANDLE outDebuggProgram;
	LPCWSTR fileDebuggOut = (LPCWSTR)_T("DebugOut.log");
	outDebuggProgram = CreateFile(fileDebuggOut,FILE_APPEND_DATA,FILE_SHARE_READ | FILE_SHARE_WRITE,&sa,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(outDebuggProgram == INVALID_HANDLE_VALUE)
		cout << "error created stdin output file" << endl;
	
	infoStart.dwFlags |= STARTF_USESTDHANDLES;
	infoStart.hStdInput = NULL;
	infoStart.hStdError = outDebuggProgram;
	infoStart.hStdOutput = outDebuggProgram; // try to redirect stdout
	
	ZeroMemory(&infoStart, sizeof(infoStart)); // fill-0 della porzione di memoria indicata (?)
	infoStart.cb = sizeof(infoStart); // grandezza della struttura in byte
	ZeroMemory(&infoProcess, sizeof(infoProcess)); //  x2 (?)

	//LPTSTR ProcToDebug = (LPTSTR)_T("ProcToDebug.exe");
	//LPTSTR ProcToDebug = (LPTSTR)_T("a.exe");
	LPTSTR ProcToDebug = (LPTSTR)_T("DebugPy.exe");

	if (!CreateProcess(ProcToDebug, NULL, NULL, NULL, FALSE,DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &infoStart, &infoProcess)) {
		cout << "fail creating process...exit "<< GetLastError() << endl;
		exit(0);
	}

	cout << "Process to debug created " << endl;
	
	hProcess = infoProcess.hProcess;
	//debugger loop
	DEBUG_EVENT debug_event = { 0 };
	DWORD dwContinueStatus;

	
	cout << "enter debugger loop" << endl;

	while (1) {

		// inizio ciclo aspettando un debug event
		
		if (!WaitForDebugEvent(&debug_event, INFINITE)) {
			cout << "error wait debug event" << endl;
			exit(1);
		}
		
		
		//cout << "[Main] on the debug loop.." << endl;
		Sleep(1000);
		
		// qui mettere comandi e chiamata alle funzioni corrispondenti e lo switch case in per il tipo di evento generato
		// il primo evento coincide con la creazione del processo
		dwContinueStatus = handleDebugEvent(&debug_event);
		if (dwContinueStatus == -1) {
			cout << "the process under debug exit" << endl;
			break;
		}
		//cout << "now restart process to debug" << endl;
		// procedo con il prossimo debug event
		if (!ContinueDebugEvent(debug_event.dwProcessId,debug_event.dwThreadId, dwContinueStatus = DBG_CONTINUE)) { 
			cout << "error continue debug event" << endl;
			exit(1);
		}

	}
	CloseHandle(hProcess);
	cout << "exit" << endl;
	return 1;
}

DWORD handleDebugEvent(LPDEBUG_EVENT debug_event) {

	// qui la logica del fault injector
	DWORD contStatus = DBG_EXCEPTION_NOT_HANDLED; // significa eccezione non gestita, se invece si gestisce l'eccezione usare DBG_CONTINUE
	//cout << "event code :" << debug_event->dwDebugEventCode << endl;
	switch (debug_event->dwDebugEventCode) {

			case CREATE_PROCESS_DEBUG_EVENT:{
				cout << "created debugee process "<< debug_event->dwProcessId << endl;
				BOOL bSuccess = SymInitialize(hProcess, NULL, false); // non sono sicuro che va qui
				break;
				};
			case CREATE_THREAD_DEBUG_EVENT: {
				cout << "thread created with id " << debug_event->dwThreadId << endl;
				
				break;
			}
			case EXCEPTION_DEBUG_EVENT: {
				cout << "EXCEPTION_DEBUG_EVENT ->";
				// scrivere tipo di eccezzione  per capire che eccezioni crea il programma scritto
				EXCEPTION_DEBUG_INFO& Exception = debug_event->u.Exception;
				
				//flip();

				switch (Exception.ExceptionRecord.ExceptionCode) {

					case EXCEPTION_BREAKPOINT: {
						cout << "EXCEPTION_BREAKPOINT -> ";
						if (Exception.dwFirstChance) // sull articolo il loop e' diverso ma il senso e' lo stesso
						{


							if (1)
								//flip();

							cout << "first chance" << endl;
							// Handle il BP del main
							// setto la flag SingleStep
							if (!tfSetted) {

								CONTEXT newThreadContext;
								newThreadContext.ContextFlags = CONTEXT_ALL;
								
								HANDLE h = OpenThread(THREAD_ALL_ACCESS,TRUE , debug_event->dwThreadId);
								if (!GetThreadContext(h, &newThreadContext)) {
									cout << "fail to get context code" << GetLastError() <<endl;
									break;
								}
								
								newThreadContext.EFlags |= 0x100; // setto tf

								if (!SetThreadContext(h,&newThreadContext)) {
									cout << "fail to set context code :" << GetLastError() << endl;
								}
								tfSetted = TRUE;
							}
							
						}
						else
						{
							cout << "second chance" << endl;
						}
						break;
				}
					case EXCEPTION_SINGLE_STEP: {
						cout << "EXCEPTION_SINGLE_STEP (mettere qui inserimento breakpoint nel main)" << endl;
						
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
				cout << "RIP EVENT the process has terminated apparently for no reason" << endl;
				break;
			}
			case EXIT_PROCESS_DEBUG_EVENT: {
				cout << "process with id "<< debug_event->dwProcessId << " exit with code " << debug_event->u.ExitProcess.dwExitCode << endl;
				contStatus = -1;
				break;
			}
		
			case EXIT_THREAD_DEBUG_EVENT: {
				cout << "thread "<< debug_event->dwThreadId << " exit with code "<< debug_event->u.ExitThread.dwExitCode << endl;
				
				break;
				
			}
			case LOAD_DLL_DEBUG_EVENT:{
				
				if (!GetFileNameFromHandle(debug_event->u.LoadDll.hFile)) {
					cout << "can't load name of dll" << endl;
				}
				// try to extract symbols from dll
				
				DWORD64 dwBase = SymLoadModule64(hProcess,NULL,(PCSTR) dllName,0, 
											(DWORD64) debug_event->u.LoadDll.lpBaseOfDll,
											debug_event->u.LoadDll.nDebugInfoSize);
				
				_tprintf(TEXT("Loaded dll name  %s\n"), dllName);


				cout << " at adress :" << debug_event->u.LoadDll.lpBaseOfDll;
				
				IMAGEHLP_MODULE64 infoModule;
				infoModule.SizeOfStruct = sizeof(infoModule);
				bool done = SymGetModuleInfo64(hProcess, dwBase, &infoModule);
				if (done) {


					cout << " symbol loaded" << endl;
				}
				else {
					cout << "fail to get symbol error: "<< GetLastError() << endl;

				}
				
				break; 
			}
			case UNLOAD_DLL_DEBUG_EVENT: {
				cout << "UNLOAD_DLL_DEBUG_EVENT" << endl;
				break; 
			}
			case OUTPUT_DEBUG_STRING_EVENT:{
				cout << "OUTPUT_DEBUG_STRING_EVENT" << endl;
				break; 
			}
	
	}

	return contStatus;

}

void flip() {
	LPBYTE buf = nullptr;
	SIZE_T byteToRead = 512;
	SIZE_T byteRead = 0;
	if (!ReadProcessMemory(hProcess, 0, &buf, byteToRead, &byteRead)) {
		cout << "fail to read process memory"<< endl;
		return ;
	}

	cout << "buffer :" << buf << endl;

}
