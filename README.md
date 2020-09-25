# FaultInjector

DONE:
- Trycopyfile (cambiare nome magari) legge il file binario e lo riscrive in un altro file

TODO:
- Bufferizzare la scrittura del file per modificare righe
- Trovare una funzione nel file binario


source:
- https://gist.github.com/endolith/147203


Appunti chiamata 25/09/2020

File lista di guasti 
- Tempo 
- Luogo
- Output

Tipi di output 
- Output corretto 
- Output sbagliato 
- Crash
- ENG (non finisce mai) -> guarda tempo di esecuzione con timeout 

Selezione localita' dei guasti iniettati
- Tutta la memoria 
- Dati/Istruzioni 
- Main/Funzione 
- Stack/Heap

TODOs
- Avere accesso tutti gli indirizzi di memoria espliciti del programma, per poter scegliere dove bloccare e dove iniettare -> DWARF sembra la via
- Formattare output log nei tipi di output sopra


Vedere rappresentazione DWARF ( gcc -g ) per sezioni del codice 
https://blog.tartanllama.xyz/writing-a-linux-debugger-elf-dwarf/

// qt (GUI) 
// segnalazione ad eventi -> 
// bus library 











