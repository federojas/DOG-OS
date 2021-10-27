#include <pipes.h>
#include <semaphores.h>
#include <prints.h>

#define MAX_PIPES 8

t_pipe pipes[MAX_PIPES];

int initialSemId = 1000;

static int createPipe(int pipeId);
static int pipeWriter(char c, int idx);
static int getNewIndex();
static int getIndex(int pipeId);

int pipeOpen(int pipeId) {
    int idx;
    if ((idx = getIndex(pipeId)) == -1) {
        idx = createPipe(pipeId);
        if (idx == -1) {
            return -1;
        } 
    }
    pipes[idx].totalProcesses++;
    return pipeId;
}

int pipeWrite(int pipeId, char *str) {
    int idx = getIndex(pipeId);
    if (idx == -1) {
        return -1;
    } 
    int i;
    while (str[i] != 0) {
        pipeWriter(str[i], idx);
        i++;
    }
    return pipeId;
}

int pipeRead(int pipeId) {
    int idx = getIndex(pipeId);

    if (idx == -1) {
        return -1;
    }

    t_pipe * pipe = &(pipes[idx]);

    char c;

    semWait(pipe->readLock);

    c = pipe->buffer[pipe->readIndex];
        //circular buffer
    pipe->readIndex = (pipe->readIndex + 1) % PIPE_BUFFER_SIZE;

    semPost(pipe->writeLock);

    return c;
}

int pipeClose(int pipeId) {
    int idx = getIndex(pipeId);
    if (idx == -1) {
        return -1;
    }

    t_pipe * pipe = &(pipes[idx]);

    pipe->totalProcesses--;

    if (pipe->totalProcesses > 0) {
        return 1;
    }

    semClose(pipe->readLock);
    semClose(pipe->writeLock);

    pipe->state = EMPTY;

    return 1;
}



static int pipeWriter(char c, int idx) {
    t_pipe *pipe= &(pipes[idx]);
    
    semWait(pipe->writeLock);

    pipe->buffer[pipe->writeIndex] = c;
    //circular buffer
    pipe->writeIndex = (pipe->writeIndex + 1) % PIPE_BUFFER_SIZE;

    semPost(pipe->readLock);

    return 0;

}

static int getIndex(int pipeId) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].state == IN_USE && pipes[i].id == pipeId) {
            return i;
        }
    }        
    return -1;
}

static int getNewIndex() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].state == EMPTY) {
            return i;
        }
    }    
    return -1;
}

static int createPipe(int pipeId) {
    int idx;

    if ((idx = getNewIndex()) == -1) {
        return -1;
    }

    t_pipe * pipe = &(pipes[idx]);

    pipe->id = pipeId;
    pipe->state = IN_USE;
    pipe->readIndex = pipe->writeIndex = pipe->totalProcesses = 0;

    if ((pipe->readLock = semOpen(initialSemId++, 0)) == -1) {
        return -1;
    }
    if ((pipe->writeLock = semOpen(initialSemId++, PIPE_BUFFER_SIZE)) == -1) {
        return -1;
    }

    return pipeId;
}

void pipeStatus() {
    printf("\n\nActive Pipe Status\n\n");
    for (int i = 0; i < MAX_PIPES; i++) {
        t_pipe pipe = pipes[i];
        if (pipe.state == IN_USE) {
            printf("Pipe ID: %d\n", pipe.id);
            printf("    Amount of attached processes: %d\n", pipe.totalProcesses);
            printf("    Read semaphore: %d\n", pipe.readLock);
            printf("    Write semaphore: %d\n", pipe.writeLock);
            printf("    Pipe buffer content: ");
            for (int i = pipe.readIndex; i != pipe.writeIndex; i = (i + 1) % PIPE_BUFFER_SIZE) {
                printf("%s", pipe.buffer[i]);
            }
        }
    }
    printf("\n\n");
}