injector: main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h  ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h ./AddressSelector/AddressSelector.h ./AddressSelector/AddressSelector.cpp
	g++ -g  -pthread -std=c++17 main.cpp  ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h ./AddressSelector/AddressSelector.h ./AddressSelector/AddressSelector.cpp -o injector





objectfiles: ./debugee/prova.c 
	gcc -g ./debugee/prova.c -o ./debugee/prova
	objdump -dl ./debugee/prova > ./Extractor/ObjectFiles/prova/objdump
	dwarfdump ./debugee/prova > ./Extractor/ObjectFiles/prova/dwarfdump

