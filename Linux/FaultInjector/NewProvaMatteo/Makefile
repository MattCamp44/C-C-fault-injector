injector: main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h objectfiles ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h ./AddressSelector/AddressSelector.h ./AddressSelector/AddressSelector.cpp
	g++ -g -pthread -std=c++17 main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h ./AddressSelector/AddressSelector.h ./AddressSelector/AddressSelector.cpp -o injector





objectfiles: ./debugee/prova.c ./Extractor/ObjectFiles/prova/objdump ./Extractor/ObjectFiles/prova/dwarfdump
	gcc -g ./debugee/prova.c -o ./debugee/prova
	objdump -d ./debugee/prova > ./Extractor/ObjectFiles/prova/objdump
	dwarfdump ./debugee/prova > ./Extractor/ObjectFiles/prova/dwarfdump

./Extractor/ObjectFiles/prova/objdump:
	touch ./Extractor/ObjectFiles/prova/objdump

./Extractor/ObjectFiles/prova/dwarfdump:
	touch ./Extractor/ObjectFiles/prova/dwarfdump
