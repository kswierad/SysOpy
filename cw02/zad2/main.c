#define __USE_XOPEN
#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <ftw.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <string.h>


char* path;
char* operant;
char* date;

char* get_permissions(const struct stat* stat_buffer) {
    char* permissions = malloc(sizeof(char) * 11);
    permissions[0] = S_ISDIR(stat_buffer->st_mode) ? 'd' : '-';
    permissions[1] = (stat_buffer->st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (stat_buffer->st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (stat_buffer->st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (stat_buffer->st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (stat_buffer->st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (stat_buffer->st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (stat_buffer->st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (stat_buffer->st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (stat_buffer->st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
    return permissions;
}

void not_nftw(const char *path, int (*fn)(const char *,
       const struct stat *, int, struct FTW *), int fd_limit, int flags){
           //printf("%s \n",path);
           
           DIR* handle = opendir(path);
           if(handle==NULL){
               printf("DOSTOŁCH NULA \n");
               return;
           }
           struct dirent* current = readdir(handle);
           while(current!=NULL){
                int path_name_len = 0;
                path_name_len = path_name_len + strlen(path);
                path_name_len = path_name_len + strlen(current->d_name);
                char * next_path = calloc(path_name_len+2,sizeof(char));
                strcat(strcat (strcat(next_path,path),"/"), current->d_name);
              // printf("TO LECIM %s\n", current->d_name);
                if(strcmp(current->d_name, "..") == 0
                    || strcmp(current->d_name, ".") == 0) {
                        current = readdir(handle);
                        continue;
                    }
                if(current->d_type == DT_DIR) not_nftw(next_path,*fn,fd_limit,flags);
                if(current->d_type == DT_REG){
                    
                    struct stat* stat_buffer = malloc(sizeof(struct stat));
                    stat(next_path, stat_buffer);
                    
                    fn(next_path,stat_buffer,FTW_F,NULL);
                }         
                current = readdir(handle);    
            
            }   
       }

void print_file_info(const struct stat* stat_buffer, char* abs_path) {
    char* permissions =  get_permissions(stat_buffer);
    printf("%s\t%lld\t%s\t%s \n",
           abs_path,
           (long long int)stat_buffer->st_size,
           permissions,
           ctime(&(stat_buffer->st_mtime))
    );
    free(permissions);
}

int compare_dates(char* user_date, char* operant, time_t file_date){
    struct tm* tajm = malloc(sizeof(struct tm));


    strptime(user_date, "%Y-%m-%d %H:%M:%S", tajm);

    time_t timet = mktime(tajm);
    if(operant[0]=='='){
        return fabs(difftime(timet,file_date))> 0.001 ? 1 : 0;
    } else if(operant[0]=='>'){
        return difftime(timet,file_date) > 0 ? 1 : 0;
    } else if(operant[0]=='<'){
        return difftime(timet,file_date) < 0 ? 1 :0;
    } else {
        printf("Wrong operant \n");
        exit(1);
    }
}

int process_el(const char *fpath,const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    
    
    char * absolutepath = calloc(256,sizeof(char));
    realpath(fpath,absolutepath);
    //printf("%s \n",absolutepath);
    
    if(typeflag == FTW_F && compare_dates(date,operant,sb->st_mtime)){
        //printf("%s \n",absolutepath);
        print_file_info(sb,absolutepath);
    }
    free(absolutepath);
    return 0;
}








int main(int argc,char* argv[]){

    path = argv[1];
    operant = argv[2];
    date = argv[3];
    //nftw(path,&process_el,5,FTW_F);
    not_nftw(path,&process_el,5,FTW_F);

    return 0;
}
