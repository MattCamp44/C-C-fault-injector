#include "Extractor.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include "../InstructionObject/InstructionObject.h"

using namespace std;

#define FIND_SYMBOL_STRING "DW_TAG_subprogram"

vector<InstructionObject> ExtractAddresses(FunctionObject functionobject, unsigned long int base, char * progname){
    
    
    // cout << "Extract addresses with:" << functionobject.getlinkagename() << endl;
    vector<string> addresses;
    vector<unsigned long> addresses_ui;
    vector<InstructionObject> Instructions;
    vector<int> Lengths;
    fstream objdumpfile;

    string dumpPath = "./Extractor/ObjectFiles/";

    string prognamestring = progname;

    string progNameWithoutPath = prognamestring.substr(prognamestring.find_last_of("/\\") + 1);

    dumpPath.append(progNameWithoutPath);

    
    dumpPath.append("/objdump");

    // dumpPath.append(prognamestring);

    // cout << dumpPath << endl;

    objdumpfile.open(dumpPath,ios::in);


    string line;

    string functiObjectLinkageName = functionobject.getlinkagename();
    functiObjectLinkageName.append(":");
    
    int instructionLength;

    while(getline(objdumpfile, line) ) { 
       if (line.find(functiObjectLinkageName, 0) != string::npos) {
            // cout << "Found in objdump: " << functionobject.getlinkagename() << endl;

            while(line != ""){
                getline(objdumpfile, line) ;
                instructionLength = 0;
                if(line!= "" ){
                    
                    
                    addresses.emplace_back(line.substr(2,6));
                    // cout << line.substr(10,22) ;

                    for(char c : line.substr(10,22))
                        if(c != ' ')
                            instructionLength++;
                    // cout << instructionLength - 1 << endl;
                    Lengths.emplace_back(instructionLength - 1);

                    
                }
            }
            if(addresses.size() != Lengths.size()){
                cout << "Different sizes\n";
                
            }

            for(auto s = 0; s<  addresses.size(); s++){
                //cout << s << endl;
                //addresses_ui.emplace_back(stoull(s,nullptr,16) + base );
                std::string::size_type sz = 0;
                // addresses_ui.emplace_back(stoull(s,&sz,16) );
                cout << addresses[s] << endl;
                Instructions.emplace_back(InstructionObject(stoull(addresses[s],&sz,16),Lengths[s]));
            }
            return Instructions;

            }

        }
    
    return Instructions;
    
    
    }





vector<FunctionObject> ExtractFunctionNames(fstream& ObjDumpFile, unsigned long int base , char * progname){

    vector<FunctionObject> FunctionObjects;

    string line;
    string linkagename;
    string functionname;
    int maxiter = 0;
        while(getline(ObjDumpFile, line) ) { 
                
            // cout << line << endl;

           if (line.find(FIND_SYMBOL_STRING, 0) != string::npos) {
                    // getline(ObjDumpFile, line); 
                    // getline(ObjDumpFile, line); 
                    while(line.find("DW_AT_name",0) == string::npos && maxiter++ < 10) getline(ObjDumpFile, line);
                    if(maxiter == 10) cout << "error\n";
                    maxiter = 0;
                    cout << line << endl;
                    functionname = line.substr(50);
                    // cout << "Function name:" << functionname << endl;
                    functionname.erase( remove( functionname.begin(), functionname.end(), '"') , functionname.end() );

                    if(functionname.find("main") == string::npos){

                        // getline(ObjDumpFile, line);
                        // cout << "Check lines" << line << endl;
                        // getline(ObjDumpFile, line); 
                        // cout << "Check lines" << line << endl;
                        // getline(ObjDumpFile, line); 
                        // cout << "Check lines" << line << endl;

                        // linkagename = line.substr(50); 
                        // cout << "LInkagename:" << linkagename << endl;
                        // linkagename.erase( remove( linkagename.begin(), linkagename.end(), '"') , linkagename.end() );
                        linkagename = "<" + functionname + ">";


                }
                    else 
                        linkagename = "<main>";
   

                    vector<InstructionObject> addresses = ExtractAddresses(FunctionObject(functionname,linkagename),base, progname);
                    FunctionObjects.emplace_back(FunctionObject(functionname,linkagename,addresses));

            }

            
        
    
        }


    ObjDumpFile.close();


    return FunctionObjects;


    }


unsigned long int getBaseAddress(int pid){

    char curdir[5000];
    getcwd(curdir,sizeof(curdir)); 

    //string commandStr = "cat /proc/";
    string commandStr = "/proc/";

    string pid_s = to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps");
    fstream mapsfile;

    mapsfile.open(commandStr,ios::in);



    
    string a;
    string baseaddr;
    
    unsigned long base;

    getline(mapsfile,a);

    
    
    baseaddr = a.substr(0,8);

    std::string::size_type sz = 0; 

    // base = stoull(baseaddr,&sz,16);

    base =0;

    return base;

}





vector<FunctionObject> extractObjects(int pid,char * progname){

    fstream ObjDumpFile;

    string dumpPath = "./Extractor/ObjectFiles/";


    string prognamestring = progname;

    string progNameWithoutPath = prognamestring.substr(prognamestring.find_last_of("/\\") + 1);

    dumpPath.append(progNameWithoutPath);

    
    dumpPath.append("/dwarfdump");

    // cout << dumpPath << endl;

    ObjDumpFile.open(dumpPath,ios::in);

    if(!ObjDumpFile.good()){
        cout << "dwarfdump file not found. Run make objectfiles debugee={your program name} to generate it\n";

    }




    unsigned long int base = getBaseAddress(pid);

    vector<FunctionObject> FunctionObjectVector = ExtractFunctionNames(ObjDumpFile, base , progname);

    char curdir[500];
    getcwd(curdir,sizeof(curdir)); 

    

    

    return FunctionObjectVector;


}








