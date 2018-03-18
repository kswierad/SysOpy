#include "libfilesys.h"
#include <stdio.h>


int main(){

    //for(int i=0;i<256;i++) printf("%c %d |",i,i);

    generate_file("test.txt",10,5);
    copy_file("test.txt","test2.txt",10,5);
    sort_file("test.txt",10,5);
    return 0;
}