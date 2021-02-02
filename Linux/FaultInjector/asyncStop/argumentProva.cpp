#include <iostream>

int main(int argc, char ** argv){


    if(argc > 2){
        std::cout << "> 2" << std::endl;
    }else{
        std::cout << "< 2" << std::endl;
    }

    return 0;

}