#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void put_char(char c);

//IO functions
int _write(int , char *, int);
int _read(int , char *, int);

int _lseek(int, int, int);
int isatty(int);

int _close(int);

caddr_t _sbrk(int);
