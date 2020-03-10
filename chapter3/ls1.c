#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

void do_ls(char []);

int main(int ac, char* args[])
{
    if (ac == 1)
        do_ls(".");
    else
        while ( --ac ) {
            printf("%s, \n", * ++args);
            do_ls(*args);
        }

    return 0;
}

void do_ls(char dirname[])
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "ls1 can not open %s", dirname);
    }else{
        while ((direntp = readdir(dir_ptr)) != NULL) {
            printf("%s \n", direntp->d_name);
        }
        closedir(dir_ptr);
    }
}
