#include <commands.h>
#include <libc.h>
#include <memoryTest.h>
#include <priorityTest.h>
#include <processTest.h>
#include <semaphoreTest.h>
#include <shell.h>
#include <stdint.h>
#include <userSyscalls.h>


static int getCommandArgs(char *userInput, char *command,
                          char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
static void shellWelcomeMessage();
static void shellExecute();
static int getCommandIdx(char *command, int *commandType);
static void initializeShell();
static void initializeCommands();
static void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
static void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
static void helpTest(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void printRow(char *str1, char *str2, int firstRow);
void printCol(char *str, int width);
void printDivider();

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
    {&helpTest, "/helpTest", "Informacion acerca de los tests"},
};

static t_command testCommands[TEST_COMMMAND_COUNT] = {
    {&testMemoryWrapper, "/memtest", "Testeo de memory manager"},
    {&testProcessesWrapper, "/proctest", "Testeo de process manager"},
    {&testPriorityWrapper, "/priotest",
     "Testeo de prioridad de process manager"},
    {&testSyncWrapper, "/semtest", "Testeo de semaforos con uso"},
    {&testNoSyncWrapper, "/nosemtest", "Testeo de semaforos sin uso"},
};

static t_shell shellData;

void startShell(int argc, char **argv) {
  shellWelcomeMessage();
  initializeShell();
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
  for (int j = 0; j < TEST_COMMMAND_COUNT; j++) {
      shellData.testCommands[j].commandFn = testCommands[j].commandFn;
          shellData.testCommands[j].name = testCommands[j].name;
    shellData.testCommands[j].description = testCommands[j].description;
  }
}

static void shellExecute() {
  char command[BUFFER_SIZE] = {0};
  char argv[MAX_ARGUMENTS][BUFFER_SIZE];
  char userInput[BUFFER_SIZE] = {0};
  int argc = 0;

  while (1) {
    printUser();

    userInput[0] = 0;

    scanf("%s", userInput);

    argc = getCommandArgs(userInput, command, argv);

    if (argc == -1) {
      printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos "
             "permitida es: %d.\n\n",
             MAX_ARGUMENTS);
    }
    int *commandType = 0;
    int commandIdx = getCommandIdx(command, commandType); 

    if (commandIdx >= 0) {
      if (*commandType == HELP_MAIN) shellData.commands[commandIdx].commandFn(argc, argv);
      else if (*commandType == HELP_TEST) shellData.testCommands[commandIdx].commandFn(argc, argv);
      
    } else {
      printf("\nComando invalido: use /help\n\n");
    }
  }
  return;
}

static int getCommandIdx(char *command, int * commandType) {
  for (int i = 0; i < COMMAND_COUNT; i++) {
    if ((strcmp(shellData.commands[i].name, command)) == 0) {
      *commandType = HELP_MAIN;
      return i;
    }
  }
  for (int j = 0; j < TEST_COMMMAND_COUNT; j++) {
    if ((strcmp(shellData.testCommands[j].name, command)) == 0) {
      *commandType = HELP_TEST;
      return j;
    }
  }
  return -1;
}

static int getCommandArgs(char *userInput, char *command,
                          char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
  int argc = 0;
  int i = 0;
  for (i = 0; userInput[i] != 0 && userInput[i] != ' '; i++) {
    command[i] = userInput[i];
  }
  command[i] = 0;
  int argIdx = 0;

  for (int j = 0; j < MAX_ARGUMENTS; j++) {
    argv[j][0] = 0;
  }

  while (userInput[i] != 0 && argc < MAX_ARGUMENTS) {
    i++;
    argIdx = 0;
    for (; userInput[i] != ' ' && userInput[i] != 0; i++, argIdx++) {
      argv[argc][argIdx] = userInput[i];
    }
    argv[argc][argIdx] = 0;
    argc++;
  }
  if (argc == MAX_ARGUMENTS && userInput[i] != 0)
    return -1;
  return argc;
}

static void shellWelcomeMessage() {
  printf("\n                 Le damos la bienvenida a\n\n");
  logo(0, 0);
  printf("\n         Sistemas Operativos --- 2C 2021\n\n");
  printf("\n  Utilice el comando /help para obtener un manual de "
         "usuario.\n\n\n\n");
}

static void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
  if (argc != 1) {
    printf("\nCantidad invalida de argumentos.\n\n");
    return;
  }
  if (strlen(argv[0]) > USER_SIZE - 1) {
    printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n",
           USER_SIZE - 1);
    return;
  }
  strcpy(shellData.userName, argv[0]);
  setFirstChange(1);
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

void printDivider() {
  printf("+");
  for (int i = 0; i < C1_WIDTH+2; i++)
    printf("-");
  printf("+");
  for (int j = 0; j < C2_WIDTH+2; j++)
   printf("-");
  printf("+\n");
}

void printHelpTable() {
  printDivider();
  printRow("Comando", "Descripcion", 1);
  printDivider();
  for (int i = 0; i < COMMAND_COUNT-1 ; i++) {
    printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
  printDivider();
}

void printHelpTestTable() {
  printDivider();
  printRow("Comando", "Descripcion del test", 1);
  printDivider();
  for (int i = 0; i < TEST_COMMMAND_COUNT ; i++) {
    printRow(shellData.testCommands[i].name, shellData.testCommands[i].description, 1);
  }
  printDivider();
}


static void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
  if (argc != 0) {
    printf("\nCantidad invalida de argumentos.\n\n");
    return;
  }
  printHelpTable();
  //   printf("\nUse ctrl + tab para cambiar de pantalla.\n");
  //   printf("\nTabla de colores: \n");
  //   printf("\nBLANCO | NEGRO | ROJO | VERDE | AZUL\n");
  //   printf("  1    |   2   |  3   |   4   |  5\n");
  //   printf("\nLista de comandos: \n");
  
}

static void helpTest(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
    if (argc != 0) {
    printf("\nCantidad invalida de argumentos.\n\n");
    return;
  }
  printHelpTestTable();
}