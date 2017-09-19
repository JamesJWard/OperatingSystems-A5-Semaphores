#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

/*Prototypes*/
void runProcess(int idx);
int getSems(int idx);

/*Macros*/
#define MAXBUFFER 100
#define KEYBSEM "/keybrd"
#define SCRNSEM "/screen"

#endif