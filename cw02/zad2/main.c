#include <stdlib>
#include <ftw.h>

int (*process_el)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){

}





int main(int argc,char* argv){

    char* path = argv[1];
    nftw(path,process_el,20,0);

    return 0;
}
