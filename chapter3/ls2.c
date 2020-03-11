/*ls2.c
 * purpose list contents of directory or directories
 * action  if no args, use . else list files in args
 * note   use stat and pwd.h and grp.h
 * bug   try ./ls2 /tmp
*
*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void do_ls(char[]);
void do_stat(char*);
void show_file_info(char*, struct stat*);
void mode_to_letters(int, char[]);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);

int main(int argc, char* argv[])
{
    if (argc == 1)
        do_ls(".");
    else {
        while (--argc) {
            printf("%s:\n", *++argv);
            do_ls(*argv);
        }
    }

    return 0;
}

void do_ls(char dir_name[])
{
    DIR* dirptr;
    struct dirent* direntp;

    if ((dirptr = opendir(dir_name)) == NULL) {
        fprintf(stderr, "ls2 can not open :%s \n", dir_name);
    } else {
        while ((direntp = readdir(dirptr)) != NULL) {
            do_stat(direntp->d_name);
        }
        closedir(dirptr);
    }
}

void do_stat(char* filename)
{
    struct stat info;

    if (stat(filename, &info)) {
        perror(filename);
    } else
        show_file_info(filename, &info);
}

void show_file_info(char* filename, struct stat* info)
{
    char modestr[11];

    mode_to_letters(info->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)info->st_nlink);
    printf("%-8s ", uid_to_name(info->st_uid));
    printf("%-8s ", gid_to_name(info->st_gid));
    printf("%8ld ", (long)info->st_size);
    printf("%.12s ", 4 + ctime(&info->st_mtime));
    printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");

    if (S_ISDIR(mode))
        str[0] = 'd';
    if (S_ISCHR(mode))
        str[0] = 'c';
    if (S_ISBLK(mode))
        str[0] = 'b';

    if (mode & S_IRUSR)
        str[1] = 'r';
    if (mode & S_IWUSR)
        str[2] = 'w';
    if (mode & S_IXUSR)
        str[3] = 'x';

    if (mode & S_IRGRP)
        str[4] = 'r';
    if (mode & S_IWGRP)
        str[5] = 'w';
    if (mode & S_IXGRP)
        str[6] = 'x';

    if (mode & S_IROTH)
        str[7] = 'r';
    if (mode & S_IWOTH)
        str[8] = 'w';
    if (mode & S_IXOTH)
        str[9] = 'x';
}
#include <pwd.h>

char* uid_to_name(uid_t uid)
{
    struct passwd* pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else
        return pw_ptr->pw_name;
}

#include <grp.h>

char* gid_to_name(gid_t gid)
{
    struct group* grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else
        return grp_ptr->gr_name;
}
