#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define NRECS 16
#define NULLUT ((struct utmp*) NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmp_buf[NRECS*UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp;

int utmp_open(char *);
int reload_utmp();
struct utmp* next_utmp();
void utmp_close();
void info_show(struct utmp *);


int main()
{
    struct utmp* utbuf;
    if (utmp_open(UTMP_FILE) == -1){
        perror(UTMP_FILE);
        exit(1);
    }

    while ((utbuf = next_utmp()) != NULLUT) {
        info_show(utbuf);
    }

    utmp_close();
    return 0;
}

int utmp_open(char* filename)
{
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return fd_utmp;
}

int reload_utmp()
{
    int records = read(fd_utmp, utmp_buf, NRECS*UTSIZE);
    num_recs = records/UTSIZE;
    cur_rec = 0;
    return records;
}

struct utmp* next_utmp()
{
    struct utmp* recp;

    if (fd_utmp == -1)
        return NULLUT;

    if (cur_rec == num_recs && reload_utmp() == 0)
        return NULLUT;

    recp = (struct utmp*)&utmp_buf[cur_rec*UTSIZE];
    cur_rec++;

    return recp;
}

void utmp_close()
{
    if (fd_utmp != -1)
        close(fd_utmp);
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
