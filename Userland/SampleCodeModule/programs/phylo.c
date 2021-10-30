// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#define LEFT(i) (((i)+philosopherCount-1) % philosopherCount)
#define RIGHT(i) (((i)+1) % philosopherCount)

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

    printf("\nVamos a dejar comer a los filosofos iniciales por %d segundos.\n\n", FRONTEND_WAIT_SECONDS);
    
    char *args[] = {"Phylo Table"};
    int tablePID = newProcess(&printTable, 1, args, BACKGROUND, NULL);
    
    sleep(FRONTEND_WAIT_SECONDS);

    printf("\nYa puede agregar o retirar comensales y terminar la cena.\n\n");

    while (tableOpen) {
        char key = getChar();
        switch (key) {
            case 'a':
                if (addPhilo() == -1) {
                    printc(RED, "\nNo hay mas lugar en la mesa.\n\n");
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
            default:
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

    char index[3];
    intToStr(philosopherCount, index, 10);
    
    char * argv[] = {"philosopher", index};
    philosopher->pid = newProcess(&philoMain, 2, argv, BACKGROUND, NULL);
    
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
        int i;
        for (i = 0 ; i < philosopherCount; i++) {
            if (philosophers[i]->state == EATING) {
                putChar('E');
            } else {
                putChar('-');
            }
            putChar(' ');
        }
        putChar('\n');
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
        printCenteredHeading(1, header[i]);
    }
    printCenteredHeading(1, "");
    printCenteredHeading(1,"Bienvenido al problema de los filosofos comensales.");
    printCenteredHeading(1,"Use A para agregar un filosofo");
    printCenteredHeading(1,"Use R para remover un filosofo");
    printCenteredHeading(1,"Use Q para finalizar");
    printFullDivider();
    sleep(FRONTEND_WAIT_SECONDS);
}