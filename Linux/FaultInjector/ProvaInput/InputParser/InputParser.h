#ifndef INPUTPARSER_H
#define INPUTPARSER_H 

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

#include "../InputParameters/InputParameters.h"
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

class InputParser{
    private:
    string InputFile;
    

    public:
    InputParser(string InputFile);

    InputParameters GetInput();
};
#endif