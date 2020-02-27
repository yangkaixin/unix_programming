#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>

#define SHOWHOST

void info_show(struct utmp*);

int main()
{
    struct utmp current_record;
    int       utmp_fd;
    int       utmp_len = sizeof(current_record);

    if((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmp_fd, &current_record, utmp_len) == utmp_len) {
        info_show(&current_record);
    }

    close(utmp_fd);
    return 0;
}

void info_show(struct utmp* utmp_buf){

    if (utmp_buf->ut_type != USER_PROCESS)
        return;

    printf("%-8s", utmp_buf->ut_user);
    printf(" ");

    printf("%-8.8s", utmp_buf->ut_line);
    printf(" ");

    printf("%11.11s", ctime(&(utmp_buf->ut_tv.tv_sec)));
    printf(" ");

#ifdef SHOWHOST
    if (utmp_buf->ut_host[0] != '\0'){
        printf("%s", utmp_buf->ut_host);
        printf(" ");
    }
#endif

    printf("\n");
}
