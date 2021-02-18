injector:  main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h  ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h  ./Output_functions/comparefiles.h ./Output_functions/comparefiles.cpp ./InstructionObject/InstructionObject.cpp ./InstructionObject/InstructionObject.h
	g++ -g  -pthread -std=c++17 main.cpp  ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h ./BreakPoint/BreakPoint.h ./BreakPoint/BreakPoint.cpp ./InjectionPoint/InjectionPoint.h ./InjectionPoint/InjectionPoint.cpp ./Debugger/Debugger.cpp ./Debugger/Debugger.h  ./Output_functions/comparefiles.h ./Output_functions/comparefiles.cpp ./InstructionObject/InstructionObject.cpp ./InstructionObject/InstructionObject.h  -o injector





objectfilesprova: ./debugee/prova.c 
	gcc -g ./debugee/prova.c -o ./debugee/prova
	objdump -d ./debugee/prova > ./Extractor/ObjectFiles/prova/objdump
	dwarfdump ./debugee/prova > ./Extractor/ObjectFiles/prova/dwarfdump

objectfiles: 
	mkdir -p ./Extractor/ObjectFiles/$(debugee)/
	mkdir -p ./output/$(debugee)/
	objdump -d ./debugee/$(debugee) > ./Extractor/ObjectFiles/$(debugee)/objdump
	dwarfdump ./debugee/$(debugee) > ./Extractor/ObjectFiles/$(debugee)/dwarfdump



