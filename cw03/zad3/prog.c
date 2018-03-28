#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    if(argc == 3){
        while(1){
            argc++;
        }
    }
    for(int i=0; i < argc; i++){
        printf("%s[%d] \n",argv[i],i);
        char * array  = calloc(100000000000,sizeof(char));
        array[0] = 'a';
    }



    return 0;
}