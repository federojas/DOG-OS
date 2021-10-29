#include <libc.h>
#include <userSyscalls.h>
#include <phylo.h>
#include <commands.h>


typedef enum {
    THINKING, HUNGRY, EATING
} t_philo_state;

typedef struct t_philosofer {
    int pid;
    int sem;
    int ID;
    t_philo_state state;
} t_philosofer;

t_philosofer * philosophers[MAX_PHILOS];
static int philosopherCount = 0;
static int mutex;
static int tableOpen;

#define LEFT(i) ((i+philosopherCount-1) % philosopherCount)
#define RIGHT(i) ((i+1) % philosopherCount)

static void thinkOrEat();
static void philoMain(int argc, char ** argv);
static void takeForks(int i);
static void putForks(int i);
static void test(int i);
static int addPhilo();
static int removePhilo();
static void printTable(int argc, char **argv);
static void printPhyloHeader();

void philoProblem(int argc, char ** argv) {
    if (checkArgcWrapper(argc, 1) == -1) {
        return;
    }
    philosopherCount = 0;
    tableOpen = 1;
    semOpen(MUTEX_SEM_ID, 1);

    printPhyloHeader();

    int i = 0;
    while (i < INITIAL_PHILOS) {
        addPhilo();
        i++;
    }

    char *args[] = {"PrintTable"};
    int tablePID = newProcess(&printTable, 1, args, BACKGROUND, NULL);

    while (tableOpen) {
        char key = getChar();
        switch (key) {
            case 'a':
                if (addPhilo() == -1) {
                    printc(RED, "\nNo hay mas luagr en la mesa.\n\n");
                } else {
                    printc(GREEN, "\nSe agrego un comensal.\n\n");
                }
                break;
            case 'r':
                if(removePhilo() == -1) {
                    printc(BLUE, "\nPor favor no se vaya, ya llega el postre.\n\n");
                } else {
                    printc(RED, "\nSe retiro un comensal.\n\n");
                }
                break;
            case 'q': 
                printf("\nMesa cerrada, cuenta: $999999, esperamos hayan disrutado su\ncomida.\n\n");
                printf("\nRecuerden propina sugerida: 10%%.\n\n");
                tableOpen = 0;
                break;
        }
    }

    for (int i = 0; i < philosopherCount; i++) {
        semClose(philosophers[i]->sem);
        killProcess(philosophers[i]->pid);
        free(philosophers[i]);
    }
    killProcess(tablePID);
    semClose(MUTEX_SEM_ID);
}

        
static int addPhilo() {
    if(philosopherCount == MAX_PHILOS) {
        return -1;
    }

    semWait(mutex);
    t_philosofer * philosopher = malloc(sizeof(t_philosofer));
    if(philosopher == NULL) {
        return -1;
    }
    philosopher->sem = semOpen(FILO_SEM_ID + philosopherCount, 1);
    philosopher->state = THINKING;
    philosopher->ID = philosopherCount;

    char * argv[] = {"philosopher"};
    philosopher->pid = newProcess(&philoMain, 1, argv, BACKGROUND, NULL);
    
    philosophers[philosopherCount++] = philosopher;
    
    semPost(mutex);
    return 0;    
}

static int removePhilo() {
    if(philosopherCount == INITIAL_PHILOS) {
        return -1;
    }

    semWait(mutex);

    t_philosofer * philosopher = philosophers[--philosopherCount];
    semClose(philosopher->sem);
    killProcess(philosopher->pid);
    free(philosopher);
    
    semPost(mutex);
    return 0;
}

static void philoMain(int argc, char ** argv) {
    int i = strToInt(argv[1], 0);
    while (1) {
        takeForks(i);
        thinkOrEat();
        putForks(i);
        thinkOrEat();
    }
}

static void takeForks(int i) {
    semWait(mutex);
    philosophers[i]->state = HUNGRY;
    test(i);
    semPost(mutex);
    semWait(philosophers[i]->sem);
}

static void putForks(int i) {
    semWait(mutex);
    philosophers[i]->state = THINKING;   
    test(LEFT(i));          
    test(RIGHT(i));
    semPost(mutex);
}

static void test(int i) {
    if (philosophers[i]->state == HUNGRY && philosophers[LEFT(i)]->state != EATING && philosophers[RIGHT(i)]->state != EATING) {
        philosophers[i]->state = EATING;
        semPost(philosophers[i]->sem);
    }
}

static void thinkOrEat() {
    sleep(THINK_EAT_WAIT_SECONDS);
}

static void printTable(int argc, char **argv) {
    while (tableOpen) {
        semWait(mutex);
        char table[MAX_PHILOS];
        int i;
        for (i = 0 ; i < philosopherCount; i++) {
            if (philosophers[i]->state == EATING) {
                table[i] = 'E';
            } else {
                table[i] = '-';
            }
        }
        table[i] = '\0';
        printRow("-", table, 1);
        semPost(mutex);
        yield();
    }
}

static void printPhyloHeader() {
    
  char header[PHYLO_HEADER_HEIGHT][PHYLO_HEADER_WIDTH] = {
      "  _____  _           _                  ",
      " |  __  | |         | |             ||||",
      " | |__) | |__  _   _| | ___         ||||",
      " |  ___/| '_  | | | | |/ _           __/",
      " | |    | | | | |_| | | (_) |        || ",
      " |_|    |_| |_| __, |_| ___/         || ",
      "                __/ |                || ",
      "               |___/                 || "};

    printFullDivider();
    for (int i = 0; i < PHYLO_HEADER_HEIGHT; i++) {
        printCenteredHeading(header[i]);
    }
    printCenteredHeading("");
    printCenteredHeading("Bienvenido al problema de los filosofos comensales.");
    printCenteredHeading("Use A para agregar un filosofo");
    printCenteredHeading("Use R para remover un filosofo");
    printCenteredHeading("Use Q para finalizar");
    printFullDivider();
    sleep(3);
}

// The solution presented in Fig. 2-47 is deadlock-free and allows the maximum
// parallelism for an arbitrary number of philosophers. It uses an array, state, to keep
// track of whether a philosopher is eating, thinking, or hungry (trying to acquire
// forks). A philosopher may move into eating state only if neither neighbor is eat-
// ing. Philosopher i’s neighbors are defined by the macros LEFT and RIGHT. In
// other words, if i is 2, LEFT is 1 and RIGHT is 3.
// The program uses an array of semaphores, one per philosopher, so hungry
// philosophers can block if the needed forks are busy. Note that each process runs
// the procedure philosopher as its main code, but the other procedures, take forks,
// put forks, and test, are ordinary procedures and not separate processes.
// #define N
// #define LEFT
// #define RIGHT
// #define THINKING
// #define HUNGRY
// #define EATING
// 5
// (i+N−1)%N
// (i+1)%N
// 0
// 1
// 2
// CHAP. 2
// / * number of philosophers * /
// / * number of i’s left neighbor * /
// / * number of i’s right neighbor * /
// / * philosopher is thinking * /
// / * philosopher is trying to get for ks * /
// / * philosopher is eating * /
// typedef int semaphore;
// int state[N];
// semaphore mutex = 1;
// semaphore s[N]; / * semaphores are a special kind of int * /
// / * array to keep track of everyone’s state * /
// / * mutual exclusion for critical regions * /
// / * one semaphore per philosopher * /
// void philosopher(int i)
// {
// while (TRUE) {
// think( );
// take forks(i);
// eat( );
// put forks(i);
// }
// } / * i: philosopher number, from 0 to N−1 * /
// void take forks(int i)
// {
// down(&mutex);
// state[i] = HUNGRY;
// test(i);
// up(&mutex);
// down(&s[i]);
// } / * i: philosopher number, from 0 to N−1 * /
// void put forks(i)
// {
// down(&mutex);
// state[i] = THINKING;
// test(LEFT);
// test(RIGHT);
// up(&mutex);
// }
// / * repeat forever * /
// / * philosopher is thinking * /
// / * acquire two for ks or block * /
// / * yum-yum, spaghetti * /
// / * put both for ks back on table * /
// / * enter critical region * /
// / * record fact that philosopher i is hungry * /
// / * tr y to acquire 2 for ks * /
// / * exit critical region * /
// / * block if for ks were not acquired * /
// / * i: philosopher number, from 0 to N−1 * /
// / * enter critical region * /
// / * philosopher has finished eating * /
// / * see if left neighbor can now eat * /
// / * see if right neighbor can now eat * /
// / * exit critical region * /
// void test(i) / * i: philosopher number, from 0 to N−1 * /
// {
// if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
// state[i] = EATING;
// up(&s[i]);
// }