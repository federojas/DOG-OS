
#include <semaphores.h>
#include <memoryManager.h>

t_semaphore * semaphores;

static t_semaphore * getSemaphore(uint32_t id);
static t_semaphore * createSemaphore(uint32_t id, uint64_t initialValue);
static void addSemaphoreToList(t_semaphore * newSem) ;

int semOpen(uint32_t id, uint64_t initialValue) {

    t_semaphore * semaphore = getSemaphore(id);

    if(semaphore == NULL) {
        semaphore = createSemaphore(id, initialValue);
    }

    if(semaphore->listeningProcesses >= MAX_BLOCKED_PROCESSES) {
        return -1;
    }

    semaphore->listeningProcesses++;
    return id;
}

static t_semaphore * createSemaphore(uint32_t id, uint64_t initialValue) {
    
        t_semaphore * newSem = malloc(sizeof(t_semaphore));
        if (newSem == NULL) {
            return -1;
        }
        newSem->id = id;
        newSem->semaphoreValue = initialValue;
        newSem->blockedProcessesAmount = 0;
        newSem->listeningProcesses = 0;
        newSem->next = NULL;
        addSemaphoreToList(newSem);
        return newSem;
}

static void addSemaphoreToList(t_semaphore * newSem) {
    t_semaphore * tailSem = semaphores;
    if(tailSem == NULL) {
        semaphores = newSem;
    } else {
        while(tailSem->next != NULL) {
            tailSem = tailSem->next;
        }
        tailSem->next = newSem;
    }
}

static t_semaphore * getSemaphore(uint32_t id) {
    t_semaphore * result = semaphores;
    while(result) {
        if(result->id == id) {
            return result;
        }
        result = result->next;
    }
    return NULL;
}