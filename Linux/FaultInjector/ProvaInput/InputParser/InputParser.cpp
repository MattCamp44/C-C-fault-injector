#include "./InputParser.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#include "../InputParameters/InputParameters.h"
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;
namespace pt = boost::property_tree;

InputParser::InputParser(string InputFile){
    this->InputFile = InputFile;
};

InputParameters InputParser::GetInput(){
    pt::ptree root;
    pt::read_json(this->InputFile,root);

    try{
        int MaxWaitingTime = root.get<int>("max_waiting_time",2);
        int NumberOfInjections = root.get<int>("number_of_injection",1);
    
        vector<string> ListOfFunction;

        for(pt::ptree::value_type &func : root.get_child("functions_to_inject")){
             ListOfFunction.push_back(func.second.data());        
        }

        InputParameters in = InputParameters(ListOfFunction,MaxWaitingTime,NumberOfInjections);
        return in;
    
    }catch(const pt::ptree_error& e){
        // errore quando chiamo e.path<string>(), failed conversion use boost::any_cast
        // in questo modo crasha lo stesso
       cout << "Errore nel file input controllare correttezza dei campi ";
       return InputParameters();
    };
    
    return InputParameters();
}