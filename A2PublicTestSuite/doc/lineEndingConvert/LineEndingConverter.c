#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){

    FILE* file = fopen(argv[1], "r");
    int c;

    while((c = fgetc(file)) != -1 ) {

        if(c == '\n')
        {
            fprintf(stdout, "%s", "\r\n");
        }
        else
        {
            fprintf(stdout, "%c", c);
        }
    }

    fclose(file);

    return 0;
}