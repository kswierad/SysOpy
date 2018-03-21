#include "libfilesys.h"
#include "libfilelib.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <zconf.h>
#include <sys/times.h>

double calculate_time(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char* argv[]){

    
    if(argc<6){
        printf("Too few arguments.");
        return 1;
    }
    
    int sys;
    if(strcmp(argv[argc-1],"sys")==0){
        sys=1;
    } else if(strcmp(argv[argc-1],"lib")==0){
        sys=0;
    } else {
        printf("Wrong last argument.");
        return 1;
    }

    struct tms* tms_start = malloc(sizeof(struct tms));
    struct tms* tms_end = malloc(sizeof(struct tms));

    clock_t real_start = times(tms_start);
    clock_t real_end;

    if(!strcmp(argv[1],"generate")){
        if(sys) generate_file_sys(argv[2], strtol(argv[3],NULL,10), strtol(argv[4],NULL,10));
        else generate_file(argv[2], strtol(argv[3],NULL,10), strtol(argv[4],NULL,10));

    } else if(!strcmp(argv[1],"sort")){
        if(sys) sort_file_sys(argv[2], strtol(argv[3],NULL,10), strtol(argv[4],NULL,10));
        else sort_file(argv[2], strtol(argv[3],NULL,10), strtol(argv[4],NULL,10));

    } else if(!strcmp(argv[1],"copy")){
        if(sys) copy_file_sys(argv[2], argv[3], strtol(argv[4],NULL,10), strtol(argv[5],NULL,10));
        else copy_file(argv[2], argv[3], strtol(argv[4],NULL,10), strtol(argv[5],NULL,10));


    } else {
        printf("Wrong first argument.");
        return 1;
    }

    real_end = times(tms_end);


    printf("REAL TIME\tUSER TIME\tSYSTEM TIME\t\n");
    printf("%lf\t%lf\t%lf\n",calculate_time(real_start,real_end),calculate_time(tms_start->tms_utime,tms_end->tms_utime),calculate_time(tms_start->tms_stime,tms_end->tms_stime));
    return 0;
}