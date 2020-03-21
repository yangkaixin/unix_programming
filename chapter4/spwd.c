/* spwd.c: a simplified version of pwd
 *
 *      starts in current directory and recursively
 *      climbs up to root of filesystem, prints top part
 *      then prints current part
 *
 *      uses readdir() to get info about each thing
 *
 *      bug: prints an empty string if run from "/"
 **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

ino_t get_inode(char *);
void print_path_to(ino_t );
void inum_to_name(ino_t node_to_find, char *namebuf, int buflen);


int main()
{
    print_path_to(get_inode("."));
    putchar('\n');
    return 0;
}

void print_path_to(ino_t this_node)
{
    ino_t my_node;
    char its_name[BUFSIZ];

    if (get_inode("..") != this_node){
        chdir("..");
        inum_to_name(this_node, its_name, BUFSIZ);
        my_node = get_inode(".");
        print_path_to(my_node);
        printf("/%s", its_name);
    }
}

void inum_to_name(ino_t node_to_find, char *namebuf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(".")) == NULL){
        perror(".");
        exit(1);
    }

    while ((direntp = readdir(dir_ptr)) != NULL) {
        if (direntp->d_ino == node_to_find){
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }

    fprintf(stderr, "error looking for inum %d\n", node_to_find);
    exit(1);
}


ino_t get_inode(char *fname)
{
    struct stat info;

    if (stat(fname, &info) == -1){
        fprintf(stderr, "can not stat");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}
