
#define N_PHILOS 5
#define MAX_PHILOS 8
#define LEFT(i) ((i+N_PHILOS-1) % N_PHILOS)
#define RIGHT(i) ((i+1) % N_PHILOS)

typedef enum {
    THINKING, HUNGRY, EATING
} t_state;

typedef struct t_philosofer {
    int pid;
    int sem;
    t_state state;
} t_philosofer;

t_philosofer * philosophers[MAX_PHILOS];

void philo(int argc, char * argv[]) {
    while (1) {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
}

void take_forks(int i) {

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