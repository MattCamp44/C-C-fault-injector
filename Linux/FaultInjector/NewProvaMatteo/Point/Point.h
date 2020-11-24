#ifndef POINT_H
#define POINT_H



class Point{

    unsigned long int address;
    int pid;

    

    public:

    Point(int pid, unsigned long int address, );
    
    virtual void Enable();

};



class BreakPoint : Point {


    void Enable();


};


class InjectionPoint : Point {

    void Enable();

}







#endif









