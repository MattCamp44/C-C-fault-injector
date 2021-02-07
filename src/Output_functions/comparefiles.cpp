#include "./comparefiles.h"
#include<stdio.h>
#include<iostream>
int compareFiles() 
{ 
    FILE *fp1 = fopen("goldenoutput.txt", "r"); 
    FILE *fp2 = fopen("injectedoutput.txt", "r"); 
  
    // if (fp1 == NULL || fp2 == NULL) 
    // { 
    //    printf("Error : Files not open"); 
    //    return ; 
    // } 

    int errorOpen = 0;

    if(fp1 == NULL){
        errorOpen=1;
        std::cout << "Goldenoutput not open\n";
    }
    
    if(fp2 == NULL){
        errorOpen = 1;
        std::cout << "injectedoutput not open\n";
    }

    if(errorOpen)
        return -1;
    // fetching character of two file 
    // in two variable ch1 and ch2 
    char ch1 = getc(fp1); 
    char ch2 = getc(fp2); 
  
    // error keeps track of number of errors 
    // pos keeps track of position of errors 
    // line keeps track of error line 
    int error = 0, pos = 0, line = 1; 
  
    // iterate loop till end of file 
    while (ch1 != EOF && ch2 != EOF) 
    { 
        pos++; 
  
        // if both variable encounters new 
        // line then line variable is incremented 
        // and pos variable is set to 0 
        if (ch1 == '\n' && ch2 == '\n') 
        { 
            line++; 
            pos = 0; 
        } 
  
        // if fetched data is not equal then 
        // error is incremented 
        if (ch1 != ch2) 
        { 
            error++; 
            // printf("Line Number : %d \tError"
            //    " Position : %d \n", line, pos); 
        } 
  
        // fetching character until end of file 
        ch1 = getc(fp1); 
        ch2 = getc(fp2); 
    } 
    fclose(fp1); 
    fclose(fp2); 
    // if(error != 0)
        // printf("Total Errors : %d\n", error); 
    return error;
} 
  
// Driver code 
// int main() 
// { 
//     // opening both file in read only mode 
    // FILE *fp1 = fopen("goldenoutput.txt", "r"); 
    // FILE *fp2 = fopen("injectedoutput.txt", "r"); 
  
    // if (fp1 == NULL || fp2 == NULL) 
    // { 
    //    printf("Error : Files not open"); 
    //    exit(0); 
    // } 
  
//     compareFiles(fp1, fp2); 
  
//     // closing both file 
//     fclose(fp1); 
//     fclose(fp2); 
//     return 0; 
// } 