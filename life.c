#include <stdio.h>

int generate()

int display(...)

int main(int argc, char** argv){
    if (argc==1){
        filename="life.txt";
        generations=rows=collumns=10;
    }
    else if argc==3{
        filename="life.txt";
        rows=argv[1];
        collumns=argv[2];
        generations=10;
    }
    else if (argc==4){
        rows=argv[1];
        collumns=argv[2];
        filename=argv[3];
        generations=10;
    }
    else if (argc==5){
        rows=argv[1];
        collumns=argv[2];
        filename=argv[3];
        generations=argv[4];
    }
    else{
        fprintf(stderr, "incorrect input");
        return 1;
    }
    res=display(filename,rows,collumns,generations);
    if (res==1){
        fprintf(stderr, "generation unsuccessful");
    }
    return 0;
}
