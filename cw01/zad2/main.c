#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>
#include <zconf.h>
#include <dlfcn.h>
#include "library.h"


char* random_string(int size){
    char* base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    if(size<1) return NULL;
    int base_size = strlen(base);
    char* string = calloc(MAX_WIDTH, sizeof(char));
    for(int i=0; i < size; i++) string[i] = base[rand()%base_size];
    for(int i=size; i<MAX_WIDTH; i++) string[i]=0;
    return string;
}

void fill_array(WrappedArray *array){
    for(int i=0; i < array->height; i++){
        char* tmp = random_string(array->width);
        create_block(array,tmp,i);
    }
}

void add_number(WrappedArray* array,int n){
    for(int i=0; i < n; i++){
        create_block(array,random_string(array->width),i);
    }
}

void remove_number(WrappedArray* array, int n){
    for(int i=0; i < n; i++){
        delete_block(array,i);
    }
}

void replace_number(WrappedArray* array, int n){
    for(int i=0; i < n; i++){
        delete_block(array,i);
        create_block(array,random_string(array->width),i);
    }
}

void exec_operation(WrappedArray* array, char* operation, int number){
    if(strcmp(operation,"search")==0){
        find_block(array,number);
        return;
    }
    if(strcmp(operation,"remove")==0){
        remove_number(array,number);
        return;
    }
    if(strcmp(operation,"add")==0){
        add_number(array,number);
        return;
    }
    if(strcmp(operation,"replace")==0){
        replace_number(array,number);
        return;
    }

    printf("Wrong action");
}

double calc_time(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char**argv){

    if(argc!=10) {
        printf("Wrong number of arguments\n");
        return (1);
    }

    int is_static;
    if(strcmp(argv[3],"static")==0) {
        is_static=1;
    }
    else if(strcmp(argv[3],"dynamic")==0) is_static=0;
        else return (1);

    static struct tms start_tms_time;
    static struct tms end_tms_time;
    clock_t start_real_time;
    clock_t end_real_time;

    start_real_time = times(&start_tms_time);
    WrappedArray* array = create_array(strtol(argv[1],NULL,10),strtol(argv[2],NULL,10),is_static);
    end_real_time = times(&end_tms_time);
    printf(" Real time: %lf \n"
                   " User time: %lf \n"
                   " System time: %lf \n"
                   "for creating array\n \n",
           calc_time(start_real_time,end_real_time),
           calc_time(start_tms_time.tms_utime,end_tms_time.tms_utime),
           calc_time(start_tms_time.tms_stime,end_tms_time.tms_stime));
    start_real_time = times(&start_tms_time);
    fill_array(array);
    end_real_time = times(&end_tms_time);
    printf(" Real time: %lf \n"
                   " User time: %lf \n"
                   " System time: %lf \n"
                   "for filling array with data\n \n",
           calc_time(start_real_time,end_real_time),
           calc_time(start_tms_time.tms_utime,end_tms_time.tms_utime),
           calc_time(start_tms_time.tms_stime,end_tms_time.tms_stime));

    start_real_time = times(&start_tms_time);
    exec_operation(array,argv[4],strtol(argv[5],NULL,10));
    end_real_time = times(&end_tms_time);
    printf(" Real time: %lf \n "
                   "User time: %lf \n "
                   "System time: %lf \n"
                   "for %s operation \n \n",
           calc_time(start_real_time,end_real_time),
           calc_time(start_tms_time.tms_utime,end_tms_time.tms_utime),
           calc_time(start_tms_time.tms_stime,end_tms_time.tms_stime),
            argv[4]);

    start_real_time = times(&start_tms_time);
    exec_operation(array,argv[6],strtol(argv[7],NULL,10));
    end_real_time = times(&end_tms_time);
    printf(" Real time: %lf \n "
                   "User time: %lf \n "
                   "System time: %lf \n"
                   "for %s operation \n \n",
           calc_time(start_real_time,end_real_time),
           calc_time(start_tms_time.tms_utime,end_tms_time.tms_utime),
           calc_time(start_tms_time.tms_stime,end_tms_time.tms_stime),
           argv[6]);

    start_real_time = times(&start_tms_time);
    exec_operation(array,argv[8],strtol(argv[9],NULL,10));
    end_real_time = times(&end_tms_time);
    printf(" Real time: %lf \n "
                   "User time: %lf \n "
                   "System time: %lf \n"
                   "for %s operation \n \n",
           calc_time(start_real_time,end_real_time),
           calc_time(start_tms_time.tms_utime,end_tms_time.tms_utime),
           calc_time(start_tms_time.tms_stime,end_tms_time.tms_stime),
           argv[8]);

    return 0;
}