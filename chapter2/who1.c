#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>

#define SHOWHOST

int main()
{
    struct utmp current_reord;
    int       utmpfd;
    int       reclen = sizeof(current_reord);

    if( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
        perror(UTMP_FILE);
        exit(1);
    }

    while( read(utmpfd, &current_reord, reclen) == reclen )
        show_info(&current_reord);

    close(utmpfd);
    return 0;
}

show_info(struct utmp *utbufp)
{
    printf("%-8.8s", utbufp->ut_user);
    printf(" ");

    printf("%-8.8s", utbufp->ut_line);
    printf(" ");

    printf("%10ld", utbufp->ut_tv.tv_sec);
    printf(" ");

#ifdef SHOWHOST
    printf("%s", utbufp->ut_host);
#endif
    printf("\n");
}
