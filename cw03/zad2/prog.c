#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    if(argc == 3){
        return 1;
    }
    for(int i=0; i < argc; i++){
        printf("%s[%d] \n",argv[i],i);
    }



    return 0;
}