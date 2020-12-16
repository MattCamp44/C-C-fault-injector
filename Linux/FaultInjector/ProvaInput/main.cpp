#include <iostream>
#include "./InputParser/InputParser.h"
#include "./InputParameters/InputParameters.h"

using namespace std;

int main(){

    InputParser in = InputParser("Input.json");
    InputParameters para = in.GetInput();
    
    cout << "n injections : " << para.GetNumberOfInjections() << endl;

    return 0;
}