// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <libc.h>
#include <memoryTest.h>
#include <priorityTest.h>
#include <processTest.h>
#include <semaphoreTest.h>
#include <shell.h>
#include <stdint.h>
#include <userSyscalls.h>
#include <stdint.h>
#include <phylo.h>

#define USERLAND_INIT_PID 1

static int getCommandArgs(char* userInput, char * argv[MAX_ARGUMENTS]);
static void shellWelcomeMessage();
static void shellExecute();
static void initializeShell();
static void initializeCommands();
static int getCommandIdx(char *command);
static void changeUser(int argc, char ** argv);
static void help(int argc, char ** argv);
static void helpTest(int argc, char ** argv);
static void helpShell(int argc, char ** argv);
static void printHelpTable();
static void printHelpTestTable();

static int findPipe(int argc, char ** argv);
static void initializePipe(int pipeIndex, int argc, char ** argv);
static int handlePipe(int pipeIndex, int argc, char ** argv);
static int runPipeCmd(int argc, char ** argv, int fdin, int fdout);

static int pipeId = 70;

static t_command commands[COMMAND_COUNT] = {
    {&help, "/help", "Listado de comandos"},
    {&clear, "/clear", "Limpia la pantalla actual"},
    {&changeUser, "/user",
     "Cambia el nombre de usuario, ingrese  el nombre como un solo argumento"},
    {&getInfoReg, "/inforeg",
     "Estado de todos los resgitros, use    Ctrl + R para capturar los mismos"},
    {&getCPUFeatures, "/cpufeatures", "Caracteristicas del CPU"},
    {&exit, "/exit", "Finaliza la ejecucion"},
    {&opCode, "/opcode", "Excepcion opcode invalido"},
    {&getCurrentDayTime, "/date&time", "Fecha y hora actual"},
    {&getMem, "/printmem",
     "Volcado de memoria de 32 bytes a      partir de direccion de memoria en hexa"
     "ingresada como argumento."},
    {&divZero, "/divzero", "Excepcion division por cero"},
    {&getCPUVendor, "/cpuvendor", "ID de fabricante del CPU"},
    {&getRoots, "/roots", "Calculo de raices de una funcion      cuadratica"},
    {&logo, "/dog", "Imprime DOG-OS logo"},
    {&doge, "/doge", "DogOS animation"},
    {&changeBgColour, "/bgcolour", "Cambia el color del fondo del texto"},
    {&changeFtColour, "/ftcolour", "Cambia el color del texto"},
    {&memStatusWrapper, "/mem", "Imprime el estado de la memoria"},
    {&semStatusWrapper, "/sem", "Imprime el estado de los semaforos"},
    {&processStatusWrapper, "/ps", "Imprime el estado de los procesos"},
    {&setPriorityWrapper, "/nice", "Cambia la prioridad de un proceso"},
    {&killProcessWrapper, "/kill", "Mata un proceso"},
    {&blockProcessWrapper, "/block", "Bloquea un proceso"},
    {&unblockProcessWrapper, "/unblock", "Desbloquea un proceso"},
    {&cat, "/cat", "Imprime el texto ingresado luego de   ejecutar el comando"},
    {&loop, "/loop", "Imprime un saludo cada 3 segundos"},
    {&pipeStatusWrapper, "/pipe", "Imprime el estado de los pipes"},
    {&filter,"/filter","Filtra las vocales del texto ingresadoluego de ejecutar el comando"},
    {&wc, "/wc", "Cantidad de lineas del texto ingresadoluego de ejecutar el comando"},
    {&philoProblem, "/phylo", "Problema de filosofos comensales"},
 
  // limite
    {&testMemoryWrapper, "/memtest", "Testeo de memory manager"},
    {&testProcessesWrapper, "/proctest", "Testeo de process manager"},
    {&testPriorityWrapper, "/priotest",
     "Testeo de prioridad process manager"},
    {&testSyncWrapper, "/semtest", "Testeo de semaforos con uso"},
    {&testNoSyncWrapper, "/nosemtest", "Testeo de semaforos sin uso"},
    {&helpTest, "/helptest", "Instrucciones acerca de los tests"},
    {&helpShell, "/helpshell", "Instrucciones acerca de la shell"},
};



static t_shell shellData;

void startShell(int argc, char **argv) {
  shellWelcomeMessage();
  initializeShell();
  killProcess(USERLAND_INIT_PID);
  shellExecute();
}

void printUser() {
  int len = strlen(shellData.userName);
  len += 4;
  printf("$ ");
  sendUserData(shellData.userName, len);
  printf(" > ");
}

static void initializeShell() {
  strcpy(shellData.userName, "DefaultUser");
  initializeCommands();
}

static void initializeCommands() {
  for (int i = 0; i < COMMAND_COUNT; i++) {
    shellData.commands[i].commandFn = commands[i].commandFn;
    shellData.commands[i].name = commands[i].name;
    shellData.commands[i].description = commands[i].description;
  }
}

static void shellExecute() {
    char userInput[BUFFER_SIZE] = {0};
    int foreground;
    
    while (1) {
        printUser();
        int argc;
        int pipeIndex;
        userInput[0] = 0;
        char * argv [MAX_ARGUMENTS] = {0};
        foreground = FOREGROUND;

        if ( scanf("%s", userInput) >= BUFFER_SIZE ) {
            printf("\nMaximo de caracteres permitidos para input: %d\n\n", BUFFER_SIZE-1);
            continue;
        }
        
        argc = getCommandArgs(userInput, argv);

        if(argc == -1) {
            printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n", MAX_ARGUMENTS);
        } 

        pipeIndex = findPipe(argc, argv);

        if(pipeIndex >= 0) {
          initializePipe(pipeIndex, argc, argv);
          continue ;
        } 

        if(argv[argc - 1][0] == '&') {
          foreground = BACKGROUND;
          argc--;
        }

        int commandIdx = getCommandIdx(argv[0]);
        
        if(commandIdx >= 0) {
            newProcess((void (*)(int, char**))shellData.commands[commandIdx].commandFn, argc, (char **)argv, foreground, NULL);     
        } else {
            printf("\nComando invalido: use /help\n\n");
        }
    }
}

static int getCommandArgs(char* userInput, char ** argv) {
    int argc = 0;

    if (*userInput != ' ' && *userInput != '\0') {
        argv[argc++] = userInput;
    }
            
  
    while(*userInput != 0) {
        if(*userInput == ' ') {
            *userInput = 0;
            if( (*(userInput + 1) != ' ') && (*(userInput + 1) != 0)) {
                if(argc >= MAX_ARGUMENTS) {
                    return -1;
                }
                argv[argc++] = userInput + 1;
            }
        }
        userInput++;
    }
    return argc;
}

static int findPipe(int argc, char ** argv) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "|") == 0) {
      return i;
    }
  }
  return -1;
}

static void initializePipe(int pipeIndex, int argc, char ** argv) {
  if(pipeIndex == 0 || pipeIndex == argc - 1) {
    printf("\nPipe (|) debe ser usado entre dos comandos.\n\n");
    return ;
  }
  int pipe = handlePipe(pipeIndex, argc, argv);
  if(pipe == -1) {
    printf("\nUno de los comandos es invalido. Use /help.\n\n");
    return ;
  }
}

static int handlePipe(int pipeIndex, int argc, char ** argv) {
  char * currentArgv[MAX_ARGUMENTS];
  int currentArgc = 0;
  int pids[2];

  int pipe = pipeOpen(pipeId++);
  if (pipe == -1) {
    printf("\nError creating pipe.\n");
    return -2;
  }

  for (int i = 0; i < pipeIndex; i++) {
    currentArgv[i] = argv[i];
    currentArgc++;
  }

  pids[1] = runPipeCmd(currentArgc, currentArgv, 0, pipe);
  if (pids[1] == -1) {
    pipeClose(pipe);
    return -1;
  }

  currentArgc = 0;
  for (int i = pipeIndex + 1, j = 0; i < argc; i++, j++) {
    currentArgv[j] = argv[i];
    currentArgc++;
  }

  int endOfFile = EOF;
  pipeWrite(pipe, (char *)&endOfFile);

  pids[0] = runPipeCmd(currentArgc, currentArgv, pipe, 1);
  if (pids[0] == -1) {
    pipeClose(pipe);
    return -1;
  }

  pipeClose(pipe);
  putChar('\n');
  return 1;
}

static int runPipeCmd(int argc, char ** argv, int fdin, int fdout) {
  int fd[2];
  int commandIdx = getCommandIdx(argv[0]);
  if (commandIdx == -1) {
    return -1;
  } 

  fd[0] = fdin;
  fd[1] = fdout;

  return newProcess(shellData.commands[commandIdx].commandFn, argc, argv, FOREGROUND, fd);
}

static void shellWelcomeMessage() {
    printf("\n                 Le damos la bienvenida a\n\n");
    logo(1,0);
    printf("\n         Sistemas Operativos --- 2C 2021\n\n");
    printf("\n  Utilice el comando /help para obtener un manual de usuario.\n\n\n\n");
}


static void printHelpTable() {
  printDivider(1, C1_WIDTH, C2_WIDTH);
     setBGC(WHITE);
    setFTC(BLACK);
    printCenteredHeading(1,"Lista de comandos");
    setFTC(WHITE);
    setBGC(BLACK);

  printDivider(0, C1_WIDTH, C2_WIDTH);
  for (int i = 0; i < TEST_COMMAND_START  ; i++) {
    printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
  printDivider(0, C1_WIDTH, C2_WIDTH);
       setBGC(WHITE);
    setFTC(BLACK);
    
    printCenteredHeading(1,"Ejemplos de uso:  /c1 | /c2   /c1 &   /c1 arg1 ...");
    printCenteredHeading(1,"Use /helptest para obtener informacion de los tests");
    printCenteredHeading(1,"Use /helpshell para obtener informacion de la shell");
    setFTC(WHITE);
    setBGC(BLACK);

    printDivider(1, C1_WIDTH, C2_WIDTH);
}

static void printHelpTestTable() {
  printDivider(1, C1_WIDTH, C2_WIDTH);
  printCenteredHeading(1, "Lista de tests");
  printDivider(0, C1_WIDTH, C2_WIDTH);
  for (int i = TEST_COMMAND_START; i < COMMAND_COUNT-2; i++) {
  printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
    
  printDivider(0, C1_WIDTH, C2_WIDTH);
}

static int getCommandIdx(char *command) {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        if ((strcmp(shellData.commands[i].name, command)) == 0) {
            return i;
        }
    }
    return -1;
}


static void help(int argc, char ** argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }

  printHelpTable();
  
}

static void helpShell(int argc, char ** argv) {
    if (checkArgcWrapper(argc, 1) == -1) { 
      return ;
    }

    printf("\n");
    setBGC(WHITE);
    setFTC(BLACK);
    printCenteredHeading(0, "Instructivo para manejo de la shell");
    setFTC(WHITE);
    setBGC(BLACK);
    printf("\nUse Ctrl + TAB para cambiar de pantalla.\n");
    printf("Use Ctrl + C para terminar el proceso actual.\n");
    printf("Use Ctrl + R para capturar el valor de los registros\n");
    printf("Use Ctrl + D para obtener resultados en comandos como \n/wc o /filter\n");
    
    printf("\n");
    setBGC(WHITE);
    setFTC(BLACK);
    printCenteredHeading(0, "Tabla de colores");
    setFTC(WHITE);

    setBGC(BLACK);
    printf("\n");

    printc(WHITE, "                      Blanco   1\n");
    printf("                      ");
    setBGC(WHITE);
    printc(BLACK, "Negro    2\n");
    setBGC(BLACK);
    printc(RED, "                      Rojo     3\n");
    printc(GREEN, "                      Verde    4\n");
    printc(BLUE, "                      Azul     5\n");
    printf("\n");
}

static void helpTest(int argc, char ** argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  printHelpTestTable();
}

static void changeUser(int argc, char ** argv) {
	if (checkArgcWrapper(argc, 2) == -1) {
		return;
  }
	if (strlen(argv[1]) > USER_SIZE - 1) {
		printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n", USER_SIZE - 1);
		return;
	}
	strcpy(shellData.userName, argv[1]);
	setFirstChange(1);
}