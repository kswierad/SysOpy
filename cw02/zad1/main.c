#include "libfilesys.h"
#include <stdio.h>


int main(){

    //for(int i=0;i<256;i++) printf("%c %d |",i,i);

    generate_file("test.txt",1500,50);
    copy_file("test.txt","test2.txt",1500,50);
    sort_file("test.txt",1500,50);
    return 0;
}