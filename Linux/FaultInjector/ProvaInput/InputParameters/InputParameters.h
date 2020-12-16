#ifndef INPUTPARAMETER_H
#define INPUTPARAMETER_H

#include<string>
#include<vector>

class InputParameters{
    private:

    std::vector<std::string> ListOfFunctons;
    int MaxWaitingTime;
    int NumberOfInjections;
    
    public: 
    InputParameters(std::vector<std::string> ListOfFunctons,int MaxWaitingTime,int NumberOfInjections);
    InputParameters();

    std::vector<std::string> GetListOfFunctions();
    int GetMaxWaitingTime();
    int GetNumberOfInjections();

};

#endif