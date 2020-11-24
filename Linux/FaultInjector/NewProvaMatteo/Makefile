injector: main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h objectfiles
	g++ main.cpp ./FunctionObject/FunctionObject.h ./FunctionObject/FunctionObject.cpp ./Extractor/Extractor.cpp ./Extractor/Extractor.h -o injector

objectfiles: ./debugee/prova.c ./Extractor/ObjectFiles/prova/objdump ./Extractor/ObjectFiles/prova/dwarfdump
	gcc -g ./debugee/prova.c -o ./debugee/prova
	objdump -d ./debugee/prova > ./Extractor/ObjectFiles/prova/objdump
	dwarfdump ./debugee/prova > ./Extractor/ObjectFiles/prova/dwarfdump

./Extractor/ObjectFiles/prova/objdump:
	touch ./Extractor/ObjectFiles/prova/objdump

./Extractor/ObjectFiles/prova/dwarfdump:
	touch ./Extractor/ObjectFiles/prova/dwarfdump
