#include <commands.h>
#include <libc.h>
#include <memoryTest.h>
#include <priorityTest.h>
#include <processTest.h>
#include <semaphoreTest.h>
#include <shell.h>
#include <stdint.h>
#include <userSyscalls.h>

#define USERLAND_INIT_PID 1

static int getCommandArgs(char* userInput, char * argv[MAX_ARGUMENTS]);
static void shellWelcomeMessage();
static void shellExecute();
static void initializeShell();
static void initializeCommands();
static int getCommandIdx(char *command);
static void changeUser(int argc, char ** argv);
static void help(int argc, char ** argv);
static void printHelpTable();
static void printHelpTestTable();
static void printRow(char *str1, char *str2, int firstRow);
static void printCol(char *str, int width);
static void printDivider();

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
    {&changeBgColour, "/bgcolour", "Cambia el color del fondo del texto"},
    {&changeFtColour, "/ftcolour", "Cambia el color del texto"},
    {&memStatusWrapper, "/mem", "Imprime el estado de la memoria"},
    {&semStatusWrapper, "/sem", "Imprime el estado de los semaforos"},
    {&killProcessWrapper, "/kill", "Imprime el estado de la memoria"},
    {&processStatusWrapper, "/ps", "Imprime el estado de los procesos"},
    {&setPriorityWrapper, "/nice", "Cambia la prioridad de un proceso"},
    {&blockProcessWrapper, "/block", "Bloquea un proceso"},
    {&unblockProcessWrapper, "/unblock", "Desbloquea un proceso"},
    {&cat, "/cat", "Imprime el texto ingresado"},
    {&loop, "/loop", "Imprime un saludo cada 3 segundos"},
    {&pipeStatusWrapper, "/pipe", "Imprime el estado de los pipes"},
  // limite
    {&testMemoryWrapper, "/memtest", "Testeo de memory manager"},
    {&testProcessesWrapper, "/proctest", "Testeo de process manager"},
    {&testPriorityWrapper, "/priotest",
     "Testeo de prioridad process manager"},
    {&testSyncWrapper, "/semtest", "Testeo de semaforos con uso"},
    {&testNoSyncWrapper, "/nosemtest", "Testeo de semaforos sin uso"},
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
    int argc = 0;
    
    while (1) {
        printUser();

        argc = 0;
        userInput[0] = 0;
        char * argv [MAX_ARGUMENTS] = {0};

        scanf("%s", userInput);
        
        argc = getCommandArgs(userInput, argv);
        
        if(argc == -1) {
            printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n", MAX_ARGUMENTS);
        } 
        int commandIdx = getCommandIdx(argv[0]);
        
        if(commandIdx >= 0) {
            newProcess((void (*)(int, char**))shellData.commands[commandIdx].commandFn, argc, (char **)argv, FOREGROUND, NULL);     
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
            if( (*(userInput + 1) != ' ') &&  (*(userInput + 1) != 0)) {
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

static void shellWelcomeMessage() {
    printf("\n                 Le damos la bienvenida a\n\n");
    logo(1,0);
    printf("\n         Sistemas Operativos --- 2C 2021\n\n");
    printf("\n  Utilice el comando /help para obtener un manual de usuario.\n\n\n\n");
}

static void printDivider() {
  printf("+");
  for (int i = 0; i < C1_WIDTH+2; i++)
    printf("-");
  printf("+");
  for (int j = 0; j < C2_WIDTH+2; j++)
   printf("-");
  printf("+\n");
}

static void printHelpTable() {
  printDivider();
  printRow("Comando", "Descripcion", 1);
  printDivider();
  for (int i = 0; i < TEST_COMMAND_START ; i++) {
    printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
  printDivider();
}

static void printHelpTestTable() {
  printDivider();
  printRow("Comando", "Descripcion del test", 1);
  printDivider();
  for (int i = TEST_COMMAND_START; i < COMMAND_COUNT; i++) {
  printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
    
  
  printDivider();
}

static int getCommandIdx(char *command) {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        if ((strcmp(shellData.commands[i].name, command)) == 0) {
            return i;
        }
    }
    return -1;
}

void printRow(char *str1, char *str2, int firstRow) {
  printf("|");
  printCol(str1, C1_WIDTH);
  printCol(str2, C2_WIDTH);
  if (firstRow)
    printf("\n");
}

void printCol(char *str, int width) {
  int done = 0;
  printf(" ");
  for (int i = 0; i < width; i++) {
    if (!done) {
      if (str[i])
        printf("%c", str[i]);
      else
        done = 1;
    }
    if (done) {
      printf(" ");
    }
  }
  if (done)
    printf(" |");
  else {
    printf(" |\n");
    printRow(" ", str + width, 0);
  }
}

static void help(int argc, char ** argv) {
  if (argc != 1) {
    printf("\nCantidad invalida de argumentos.\n\n");
    return;
  }
  
  printf("\nUse ctrl + tab para cambiar de pantalla.\n");
  printf("Use ctrl + c para terminar el proceso actual.\n");
  printf("\nBLANCO | NEGRO | ROJO | VERDE | AZUL\n");
  printf("  1    |   2   |  3   |   4   |  5\n");

  printHelpTable();
  printHelpTestTable();
}

static void changeUser(int argc, char ** argv) {
	if (argc != 2) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	if(strlen(argv[1]) > USER_SIZE - 1) {
		printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n", USER_SIZE - 1);
		return;
	}
	strcpy(shellData.userName, argv[1]);
	setFirstChange(1);
}