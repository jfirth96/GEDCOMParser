#include "GEDCOMparser.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
    GEDCOMobject *obj;
    //Add more files
    char* files[] = {"testFiles/valid/minValid.ged", "testFiles/valid/simpleValid1N.ged", "testFiles/valid/nuclearFamily.ged","testFiles/valid/shakespeare.ged"};
    
    if (argc != 2){
        fprintf(stderr, "usage: MemTest fileNum, where filenum is a number between 1 and 4\n");
        exit(0);
    }
    
    int fileNum = atoi(argv[1]);
    
    if (fileNum < 1 || fileNum > 4){
        fprintf(stderr, "usage: MemTest fileNum, where filenum is a number between 1 and 4\n");
        exit(0);
    }
    
    fileNum -= 1;
    
    createGEDCOM(files[fileNum], &obj);
    deleteGEDCOM(obj);
    
    return 0;
    
}



