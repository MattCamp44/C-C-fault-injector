#include "./InputParameters.h"

#include <string>
#include <vector>

InputParameters::InputParameters(){}; // costruttore di default, lo uso in caso di errore

InputParameters::InputParameters(std::vector<std::string> ListOfFunctons,int MaxWaitingTime,int NumberOfInjections){

    this->ListOfFunctons = ListOfFunctons;
    this->MaxWaitingTime = MaxWaitingTime;
    this->NumberOfInjections = NumberOfInjections;

};

std::vector<std::string> InputParameters::GetListOfFunctions(){
    return this->ListOfFunctons;
};
int InputParameters::GetMaxWaitingTime(){
    return this->MaxWaitingTime;
};
int InputParameters::GetNumberOfInjections(){
    return this->NumberOfInjections;
};