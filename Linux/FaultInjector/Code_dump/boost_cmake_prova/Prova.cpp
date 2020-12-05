#include <ProvaConfig.h>
#include <iostream>

using namespace std;

int main(int argc,char argv[]){

    if (argc < 2) {
    // report version
        std::cout << argv[0] << " Version " << Prova_VERSION_MAJOR << "." << Prova_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
    }

    printf("START\n");
    printf("1 \n");
    printf("2 \n");
    printf("3 \n");
    printf("4 \n");
    printf("5 \n");
    printf("6 \n");
    printf("7 \n");
    printf("END\n");
}