#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
FILE* file_handle;

void* put(void* arg){
    //fseek(file_handle,((int)arg),0);
    char buffer[256];
    fgets(buffer,256,file_handle);
    usleep(((int)arg)%512);
    printf("%s",buffer);

}


int main(){

    file_handle = fopen("pan-tadeusz.txt","r");
    FILE* rand_handle = fopen("/dev/urandom","r");
    while(feof(file_handle)==0){
        pthread_t thread;
        int random; 
        fread(&random,sizeof(char),1,rand_handle);
        pthread_create(&thread,NULL,put,random);
    }

    sleep(10);

    return 0;
}
