#ifndef _PHYLO_H
#define _PHYLO_H

#include <colors.h>

#define INITIAL_PHILOS 5
#define MAX_PHILOS 10

#define FILO_SEM_ID 2000
#define MUTEX_SEM_ID 3000
#define THINK_EAT_WAIT_SECONDS 1

void philoProblem(int argc, char ** argv);

#endif