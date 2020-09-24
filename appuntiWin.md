 
## Appunti vari


- [Note](#note)
- [Appunti Giorno](#appunti-giorno)
  - [24/09/2020](#24092020)
  - [22/09/2020](#22092020)
  - [21/09/2020](#21092020)
  - [18/09/2020](#18092020)
  - [17/09/2020](#17092020)
  - [16/09/2020](#16092020)
  - [15/09/2020](#15092020)
  - [14/09/2020](#14092020)
- [Teoria](#teoria)
  - [Debug Event e Breakpoint](#debug-event-e-breakpoint)
- [Codice](#codice)
    - [17/09/2020](#17092020-1)

Soluzione :
attualmente si basa tutto sull'uso delle api messe a disposizone da windows per fare il debugger, in particolare basandosi su queste 3 risorse [1](https://www.codeproject.com/Articles/43682/Writing-a-basic-Windows-debugger) [2](https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2) [3](https://www.microsoftpressstore.com/articles/article.aspx?p=2201303)

un idea per creare i breakpoint, se il programma lavora con le api di windows basta che in qualsiasi punto del codice si chiami DebugBreak() e viene generata un eccezione che il processo debugger puo' intercettareS


Dprecato :
~~vedere [qui](http://sysprogs.com/GDBServerFoundation/dox/_simple_win32_server_2_simple_win32_server_8cpp-example.html) cosa fa questo server facendolo partire.
A quel punto si potrebbe rivedere la stessa cosa ma in seno ai softerror, ovvero cambiare cosa fanno le api e magari fornire un interfaccia per interagire con il server.
    #include "stdafx.h"
Serve scaricare questa libreria~~


Deprecato:
~~fare tutto in unix, usare la libreria [GDB/MI](https://sourceware.org/gdb/onlinedocs/gdb/GDB_002fMI.html) per interagire con il codice del programma, stopparlo scegliendo le funzioni e quant'altro.
Da GDB o da C++ riusciamo a prendere i puntatori agli indirizzi di memoria virtuale che il processo usa, tipo lo stack pointer dei thread oppure l'heap ecc.
Avendo gli indirizzi a quel punto utilizzare [asm](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm) per modificare il valore dei registri ecc.
Per attuare questa soluzione però bisogna capire come usare assembly, cosa farci di preciso ecc.
(Aspettando in tutto ciò se su stack overflow qualcuno mi suggerisce una libreria o debugger da utilizzare su windows, a quel punto si può gestire il lato modifiche HW da codice utilizzando, almeno in parte le windows api tipo debugapi.h e memory.h per il thread context, heap ecc.)~~


Deprecato:
~~1) un modo per settare i breakpoint sul programma prima di creare ed eseguire il processo (forse appoggiandosi a LLDB debugger che ha una libreria che espone un interfaccia compatibile con C++) **probabilmente devo attaccarmi al debug di gdb con delle api (https://sourceware.org/gdb/papers/libgdb2/libgdb_1.html#SEC1)**
1) capire cosa si può modificare in questo modo
2) settare un thread context e vedere che succede 
3) interpretare il thread context
4) guardate sotto nota **!!**
5) **trovare funzione migliore di CreateProcess() per attaccarsi al processo e magari vederlo meglio**~~

~~nota :
per settare i breakpoint o si usa LLDB (da vedere), oppure lo facciamo eseguendolo su un gdb che ci setta i breakpoint (lo fai all'inizio dell esecuzione), quando arriva ad un breakpoint il controllo passa all'applicazione che permette di gestire e cambiare il contenuto del context thread e anche dell'heap, visto che il context punta allo stack si può pensare di (forse ?) eseguire uno script in assembly che fa qualche cosa in particolare.
Oppure bisognerebbe capire se DebugBreak() funziona per quello che ci serve oppure no~~

Guardare anche qui per [SetBreakpoint](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-idebugcontrol3-addbreakpoint)

**!!**
Inoltre le funzioni che mette a disposizione windows per il debugger sono [qui](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/_debugger/) da li ci sono funzioni per fare quasi tutto in termini di manipolazione dei processi e dei dati annessi a quei processi

***

# Note

per far funzionare la libreria Dbghelp.h bisogna aggingere a mano la libreria al linker, in configuration properties->Configuration properties->linker->input->Additional Dependencies aggiungere **Dbghelp.h**
# Appunti Giorno
## 24/09/2020

in windows >> nm nome.exe comando che estrae i simboli dall'eseguibile

quando si vanno a caricare i simboli del main si ho notato che il tipo di simboli che viene caricato e' SymDeferred, da [qui](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/deferred-symbol-loading#:~:text=By%20default%2C%20symbol%20information%20is,it%20encounters%20an%20unrecognized%20symbol.)
si capisce che in effetti i simboli non vengono caricati fintanto che questi non vengono chiamati questa tecnica viene detta *deferred symbol loading*, quindi devo trovare in modo di caricarli indipendentemente da questo (forse c'e' un modo per disabilitare questa opzione)

## 22/09/2020

provo a implementare il BP nella prima istruzione del programma

## 21/09/2020

aggiunto progetto al repository
trovato modo di scrivere la memoria del processo, a questo punto unendo il contesto dei Thread (e dei thread, volendo memorizzarli) possiamo mettere l'errore ovunque vogliamo, quindi da dove cominciano le librerie o qualsiasi altra cosa, la cosa da fare ora e' riuscire a printare lo stack e a mettere i breakpoint (magari anche poter fare il walking del codice)

- per la grandezza del processo guardare la api GetMemoryProcessInfo che ritorna una struttura _PROCESS_MEMORY_COUNTERS  che ha un attributo che e' la grandezza del working set

- per la seconda shell scrivere classe


## 18/09/2020
Si puo' effettuare il debugger anche di codice non C/C++ (testato python)

Cerco di mettere un breakpoint nel main e provo a printare lo stack e magari a caricare i simboli

Per mettere un BP nell'entrypoint del programma devo sostituire il primo byte con 0xCC(204) che e' il codice relativo al BP.
su windbg bisogna fare il disassembly per vedere la prima istruzione e le successive

per quanto rigurada la libreria Dbghelp.h i riferimenti a quelle funzioni non sono risolti dal linker di VS e l'applicazione non parte, per fare il walking dello stack serve la stessa libreria quindi bohboh.



## 17/09/2020
continuo utilizzando debugapi per tentare di fermare il processo e farlo ripartire.
I modi per far fermare il debug consiste nell'aspettare il debug event (come la creazione di un nuovo threa/processo oppure terminate process)




## 16/09/2020

tento di eseguire il programma tramite il debugger, l'idea è quella di avviare con il debugger (GDB) il processo e settare i brakpoint da gdb, a quel punto aspettarli dal programma e quando si raggiunge si eseguono le le funzioni di Set/GetThreadContext per i context del/dei thread/s.

Se si potesse scrivere sullo stdin di un altro processo da me creato sarebbe good.

Bisogna avviare i processi in 3 shell separate altrimenti GDB non vede il processo (anzi non si vede nemmeno dal taskmanager), con flag PROCESS_DEBUG il processo non viene creato e GDB infatti non lo vede

usare FLAG -O0 -ggdb


## 15/09/2020

Vedere sopra in soluzione:

nota:
Il thread su cui eseguo le operazioni ora è quello che fa partire CreateProcess, quindi il thread dovrebbe essere quello, in sostanza dovrei rifarmi al processo.
In ogni caso essendo su windows ogni processo crea almeno un thread per eseguire il main, quindi dovrei riuscire ad attaccarmi a quel thread in caso.



## 14/09/2020

1 : prima di fare getThreadContext() bisogna sospendere il thread con SuspendThread()
2 : la struttura context che prende getThreadContext dipende dall'architettura del pc, non è detto che
ci sia nel caso AMD per questo forse da errore (code error: 998)
3 : la variante Wow64 delle funzioni ritorna lo stesso erroe 998

il context va allineato secondo il tipo di architettura e vanno settate le flag giuste prima di chiamare 
getThreadContext

private enum CONTEXT64_FLAGS : uint
{
    CONTEXT64_AMD64 = 0x100000,
    CONTEXT64_CONTROL = CONTEXT64_AMD64 | 0x01,
    CONTEXT64_INTEGER = CONTEXT64_AMD64 | 0x02,
    CONTEXT64_SEGMENTS = CONTEXT64_AMD64 | 0x04,
    CONTEXT64_FLOATING_POINT = CONTEXT64_AMD64 | 0x08,
    CONTEXT64_DEBUG_REGISTERS = CONTEXT64_AMD64 | 0x10,
    CONTEXT64_FULL = CONTEXT64_CONTROL | CONTEXT64_INTEGER | CONTEXT64_FLOATING_POINT,
    CONTEXT64_ALL = CONTEXT64_CONTROL | CONTEXT64_INTEGER | CONTEXT64_SEGMENTS | CONTEXT64_FLOATING_POINT | CONTEXT64_DEBUG_REGISTERS
}


errore risolto in questo modo :

CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_ALL;
	//PWOW64_CONTEXT ctx = {};
	if (GetThreadContext(thread,&ctx) == 0) {
		cout << "[Main] Not able to take thread context..exit :" << GetLastError() << endl;
		return 1;
	}

invece di passare LPCOntext passo &ctx e setto le flag, forse c'è da impostare un offeset dei registri 

se ne parla qui : http://www.rohitab.com/discuss/topic/43738-getthreadcontext-is-failing/

Windows 10
Rcx = 5963776
Rdx = 6492160
Works fine

probabilmente dipende dal singolo sistema, guardare su WinNT.h source code (https://source.winehq.org/source/include/winnt.h)

In ogni caso la struttura context => https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-context


    typedef struct _CONTEXT {
      DWORD64 P1Home;
      DWORD64 P2Home;
      DWORD64 P3Home;
      DWORD64 P4Home;
      DWORD64 P5Home;
      DWORD64 P6Home;
      DWORD   ContextFlags;
      DWORD   MxCsr;
      WORD    SegCs;
      WORD    SegDs;
      WORD    SegEs;
      WORD    SegFs;
      WORD    SegGs;
      WORD    SegSs;
      DWORD   EFlags;
      DWORD64 Dr0;
      DWORD64 Dr1;
      DWORD64 Dr2;
      DWORD64 Dr3;
      DWORD64 Dr6;
      DWORD64 Dr7;
      DWORD64 Rax;
      DWORD64 Rcx;
      DWORD64 Rdx;
      DWORD64 Rbx;
      DWORD64 Rsp;
      DWORD64 Rbp;
      DWORD64 Rsi;
      DWORD64 Rdi;
      DWORD64 R8;
      DWORD64 R9;
      DWORD64 R10;
      DWORD64 R11;
      DWORD64 R12;
      DWORD64 R13;
      DWORD64 R14;
      DWORD64 R15;
      DWORD64 Rip;
      union {
        XMM_SAVE_AREA32 FltSave;
        NEON128         Q[16];
        ULONGLONG       D[32];
        struct {
          M128A Header[2];
          M128A Legacy[8];
          M128A Xmm0;
          M128A Xmm1;
          M128A Xmm2;
          M128A Xmm3;
          M128A Xmm4;
          M128A Xmm5;
          M128A Xmm6;
          M128A Xmm7;
          M128A Xmm8;
          M128A Xmm9;
          M128A Xmm10;
          M128A Xmm11;
          M128A Xmm12;
          M128A Xmm13;
          M128A Xmm14;
          M128A Xmm15;
        } DUMMYSTRUCTNAME;
        DWORD           S[32];
      } DUMMYUNIONNAME;
      M128A   VectorRegister[26];
      DWORD64 VectorControl;
      DWORD64 DebugControl;
      DWORD64 LastBranchToRip;
      DWORD64 LastBranchFromRip;
      DWORD64 LastExceptionToRip;
      DWORD64 LastExceptionFromRip;
    } CONTEXT, *PCONTEXT;


#4

Ogni processo viene generato con un singolo thread, il thread in windows è l'unità base di 
sincronizzazione.
(Nota: windows non mantiene vincoli di parentela tra i processi che crea, quindi non è del tutto appropriato parlare di processo padre e figlio)

![](2020-09-14-15-26-28.png)

(Informazioni sul thread/process [model di windows](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/windows-kernel-mode-process-and-thread-manager#:~:text=A%20process%20is%20a%20software,a%20number%20that%20identifies%20it.))

Ogni thread crea una struttura dati *TLS (Thread Local Storage)* nella memoria virtuale associata al processo e mette le variabile che appartengono a lui.

![](2020-09-14-15-43-54.png)




# Teoria
## Debug Event e Breakpoint

appunti di [qui](https://www.microsoftpressstore.com/articles/article.aspx?p=2201303) e [qui](https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2)

il sistema operativo genera diversi tipi di debug events, ogni volta che viene caricato un modulo oppure quando viene generato un processo dal target del debugger e quando viene generata un eccezione.
I breakpoint sono dei *debug event*, infatti vengono generati forzando una eccezione processo nel processo target.
In questo caso le eccezioni possono essere di due tipi *single step* (ovvero quando si esegue un istruzione alla volta) oppure *breakpoint*.

Bisogna quindi capire come vengono gestite le eccezioni per gestire breakpoint e single-step.
Esistono due tipi di eccezione : *language level* e *frameworm level*.
Per la gestione delle eccezioni viene impiegata una struttura detta *Structured Exception Handling (SEH)*.
Parleremo di SEH nel caso di eccezioni generate dal sistema operativo, quindi framework level (le altre sono le eccezioni che generato i programmi user-level).
Le SEH si possno ulteriormente suddividere in due categorie : *hardware exception e software exception*, quelle hw sono quelle che coinvolgono il framwork appunto come *invalid memory access* e *divide-by-0* mentre quelle di tipo software possono essere innescate da RaiseException() che una win32API, la parola chiave thrown ad esempio viene tradotta dal compilatore usando quella api.
In questo modo che vengono generati i breakpoint e la possibilita\` di fare walking del codice; tramite le eccezzioni di tipo hw.
le parole chiave try catch e finally utilizzano le funzionalita` di SEH.

Come vengono gestiste le eccezioni tra debugger e target ? 

![](2020-09-17-12-56-36.png)

in pratica quando viene generata un eccezione ntdll.dll ovvero il dispatcher dell'eccezione notifica prima il processo debugger, questa viene detta 1-st chance notification, se il debugger decide di ignorarla a quel punto il programma viene fatto ripartire e se nemmeno lui gestisce l'eccezione viene inviata un altra notifica al debugger come last-chance che viene bloccato.

la prima chance viene semplicemente loggata mentre la seconda (almeno sul debug Windgb) blocca il debugger.
Quando si dice *breaking into the debugger* si intende che il processo target puo` intervenire con il processo sotto debugger e analizzarlo.
Questo viene fatto utilizzando DebugBreakProcess.

Per implementare i breakpoint si utilizza un tipo (utilizzato anche per le altre eccezioni) int3, i breakpoint devono essere implementati riscrivendo la memoria del processo dove viene richiesto dall'utente.

`This scheme sounds straightforward, but there is a catch: how is the debugger able to insert the int 3 instruction before the execution of the target process is resumed (using the g command) after a breakpoint hit? Surely, the debugger can’t simply insert the debug break instruction before the target’s execution is resumed because the next instruction to execute is supposed to be the original one from the target and not the int 3 instruction. The way the debugger solves this dilemma is the same way it is able to support single-stepping, which is by using the TF (“trap flag”) bit of the EFLAGS register on x86 and x64 processors to force the target thread to execute one instruction at a time. This single-step flag causes the CPU to issue an interrupt (int 1) after every instruction it executes. This allows the thread of the breakpoint to execute the original target instruction before the debugger is immediately given a chance to handle the new single-step SEH exception—which it does by restoring the debug break instruction again, as well as by resetting the TF flag so that the CPU single-step mode is disabled again.`

In sostanza il debugger come fa a sostituire l'istruzione con la int3 dopo aver preso un breakpoint ? anche perche` dovrebbe eseguire la prossima che il programma prevede, infatti lo fa settando la flag TF (TRAP FLAG) e quindi forza la cpu ad eseguire un istruzione alla volta generando un interrupt (int1) dopo ogni istruzione.



Debugbreak *{int3}*, bisogna mettere questa istruzione per ottenere un breakpoint, e va messa come prossima istruzione del processo.
Questo e\` fondamentale e bisogna farlo nell'entry point del processo ovvero la prima istruzione, in questo modo possiamo controllare l'esecuzione del programma, solitamente viene messo nel main ma non e\` l'entry point.

devo scrivere il breakpoint code, {int3} 0xCC nello spazio di memoria riservato alla prima istruzione, il primo byte (little ending?) e basta con CC.

La strategia e' (vediamo se funziona), faccio partire il processo con le flag sue e a quel punto faccio WaitForDebugEvent(), questo intercettera' la creazione del processo che viene di default visto come debugevent.
A quel punto prendo il primo indirizzo di memoria del processo (codice su articolo) 
A quel punto memorizziamo cosa c'era nel primo byte, ci scriviamo 0xCC e facciamo il flush dell'istruction cache e vado avanti con COntinueDEbugEvent

Il problema ora e' il seguente : quando il processore legge l'istruzione vede **CC** e ritorna l'eccezione, ma nel frattempo legge tutta l'istruzione, quindi devo spostare l'istruction pointer di 1 byte indietro e rimettere la vecchia istruzione (manipolo tutto con get/set thread context e la struttura CONTEXT)

il codice e i passaggi sono tutti nell'articolo (il secondo in particolare) 
(nota: servono le flag THREAD_GET_CONTEXT e THREAD_SET_CONTEXT come permessi, penso siano su process info)

TRAP FRAME e EXCEPTION_SINGLE_STEP

in questo caso non e' un breakpoint ma una flag nel processo che fa la trap ad ogni step.

Per fare il wlking del codice c'e' bisogno di implementare step-in anche dentro alle funzioni.

***

# Codice
### 17/09/2020
(preso da [qui](https://www.codeproject.com/Articles/43682/Writing-a-basic-Windows-debugger))
DBG_EXCEPTION_NOT_HANDLED al posto di DBG_CONTINUE
informa il gestore debug object che l'eccezione non e' stata gestita

    struct DEBUG_EVENT
    {
        DWORD dwDebugEventCode;
        DWORD dwProcessId;
        DWORD dwThreadId;
        union {
            EXCEPTION_DEBUG_INFO Exception;
            CREATE_THREAD_DEBUG_INFO CreateThread;
            CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
            EXIT_THREAD_DEBUG_INFO ExitThread;
            EXIT_PROCESS_DEBUG_INFO ExitProcess;
            LOAD_DLL_DEBUG_INFO LoadDll;
            UNLOAD_DLL_DEBUG_INFO UnloadDll;
            OUTPUT_DEBUG_STRING_INFO DebugString;
            RIP_INFO RipInfo;
        } u;
    };

questa e' la struttura  che descrive un debugevent e ce ne sono di 9 + 20 sottocategorie, in dwDebugEvent c'e' il codice

Un eccezione che adesso non considero ma potrebbe essere utile e' OUTPUT_DEBUG_STRING_EVENT con la corrispondente API da utilizzare dentro il codice del programma target

CREATE_PROCESS_DEBUG_INFO questo evento viene generato quando viene creato un processo, la parte interessante e' CREATE_PROCESS_DEBUG_INFO che e' una struttura : 


    struct CREATE_PROCESS_DEBUG_INFO
    {
        HANDLE hFile;   // The handle to the physical file (.EXE)
        HANDLE hProcess; // Handle to the process
        HANDLE hThread;  // Handle to the main/initial thread of process
        LPVOID lpBaseOfImage; // base address of the executable image
        DWORD dwDebugInfoFileOffset;
        DWORD nDebugInfoSize;
        LPVOID lpThreadLocalBase;
        LPTHREAD_START_ROUTINE lpStartAddress;
        LPVOID lpImageName;  // Pointer to first byte of image name (in Debuggee)
        WORD fUnicode; // If image name is Unicode.
    }
  
  ###nota quando faccio continueDebugEvent devo utilizzare il process id e il thread id che questa struttura mi da che e' (o puo' essere ?)  differente da prima perche' windows usa handle diversi per scopi differenti
