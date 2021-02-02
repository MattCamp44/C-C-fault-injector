#include "Extractor.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include<string.h>

using namespace std;

#define FIND_SYMBOL_STRING "subprogram"

vector<unsigned long> ExtractAddresses(FunctionObject functionobject, unsigned long int base, char * progname){
    
    
    cout << "Extract addresses with:" << functionobject.getlinkagename() << endl;
    vector<string> addresses;
    vector<unsigned long> addresses_ui;

    fstream objdumpfile;

    string dumpPath = "./Extractor/Objectfiles/";

    string prognamestring = progname;

    string progNameWithoutPath = prognamestring.substr(prognamestring.find_last_of("/\\") + 1);

    dumpPath.append(progNameWithoutPath);

    
    dumpPath.append("/objdump");

    // dumpPath.append(prognamestring);

    cout << dumpPath << endl;

    objdumpfile.open(dumpPath,ios::in);


    string line;

    while(getline(objdumpfile, line) ) { 
       if (line.find(functionobject.getlinkagename(), 0) != string::npos) {
            cout << "Found in objdump: " << functionobject.getlinkagename() << endl;

            while(line != ""){
                getline(objdumpfile, line) ;

                if(line!= "" ){
                    

                    addresses.emplace_back(line.substr(2,6));
                
                }
            }
            
            for(auto s : addresses){
                //cout << s << endl;
                //addresses_ui.emplace_back(stoull(s,nullptr,16) + base );
                std::string::size_type sz = 0;
                addresses_ui.emplace_back(stoull(s,&sz,16) );
            }
            return addresses_ui;

            }

        }
    
    
    
    }





vector<FunctionObject> ExtractFunctionNames(fstream& ObjDumpFile, unsigned long int base , char * progname){

    vector<FunctionObject> FunctionObjects;

    string line;
    string linkagename;
    string functionname;

        while(getline(ObjDumpFile, line) ) { 
                


           if (line.find(FIND_SYMBOL_STRING, 0) != string::npos) {
                    getline(ObjDumpFile, line); 
                    getline(ObjDumpFile, line); 
                    
                    functionname = line.substr(50);
                    cout << functionname << endl;
                    functionname.erase( remove( functionname.begin(), functionname.end(), '"') , functionname.end() );

                    if(functionname.find("main") == string::npos){

                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 

                        linkagename = line.substr(50); 
    
                        linkagename.erase( remove( linkagename.begin(), linkagename.end(), '"') , linkagename.end() );



                }
                    else 
                        linkagename = "<main>";
   

                    vector<unsigned long> addresses = ExtractAddresses(FunctionObject(functionname,linkagename),base, progname);
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

    base = stoull(baseaddr,&sz,16);

    

    return base;

}





vector<FunctionObject> extractObjects(int pid,char * progname){

    fstream ObjDumpFile;

    string dumpPath = "./Extractor/Objectfiles/";


    string prognamestring = progname;

    string progNameWithoutPath = prognamestring.substr(prognamestring.find_last_of("/\\") + 1);

    dumpPath.append(progNameWithoutPath);

    
    dumpPath.append("/dwarfdump");



    ObjDumpFile.open(dumpPath,ios::in);

    if(!ObjDumpFile){
        cout << "dwarfdump file not found. Run make objectfiles debugee={your program name} to generate it\n";

    }




    unsigned long int base = getBaseAddress(pid);

    vector<FunctionObject> FunctionObjectVector = ExtractFunctionNames(ObjDumpFile, base , progname);

    char curdir[500];
    getcwd(curdir,sizeof(curdir)); 

    

    

    return FunctionObjectVector;


}








