#include <shell.h>
#include <libc.h>
#include <commands.h>
#include <stdint.h>
#include <memoryTest.h>
#include <processTest.h>
#include <priorityTest.h>
#include <semaphoreTest.h>
#include <userSyscalls.h>

static int getCommandArgs(char* userInput, char* command, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
static void shellWelcomeMessage();
static void shellExecute();
static int getCommandIdx(char * command);
static void initializeShell();
static void initializeCommands();
static void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
static void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);

static t_command commands[COMMAND_COUNT] = {
    {&help, "/help", "Listado de comandos"},
    {&testMemory, "/memtest", "Testeo de memory manager"},
    {&testProcesses, "/proctest", "Testeo de process manager"},
    {&testPriority, "/priotest", "Testeo de prioridad de process manager"},
    {&testSync, "/semtest", "Testeo de semaforos con uso"},
    {&testNoSync, "/nosemtest", "Testeo de semaforos sin uso"},
    {&clear, "/clear", "Limpia la pantalla actual"},
    {&changeUser, "/user", "Cambia el nombre de usuario, ingrese el nombre como un solo argumento"},
    {&getInfoReg, "/inforeg", "Estado de todos los resgitros, use ctrl + r para capturar los mismos"},
    {&getCPUFeatures, "/cpufeatures", "Caracteristicas del CPU"},
    {&exit, "/exit", "Finaliza la ejecucion"},
    {&opCode, "/opcode", "Excepcion opcode invalido"},
    {&getCurrentDayTime, "/date&time", "Fecha y hora actual"},
    {&getMem, "/printmem", "Volcado de memoria de 32 bytes a partir de\ndireccion de memoria en hexa ingresada como argumento."},
    {&divZero, "/divzero", "Excepcion division por cero"},
    {&getCPUVendor, "/cpuvendor", "ID de fabricante del CPU"},
    {&getRoots, "/roots", "Calculo de raices de una funcion cuadratica"},
    {&logo, "/dog", "Imprime DOG-OS logo"},
    {&changeBgColour, "/bgcolour", "Cambia el color del fondo del texto"},
    {&changeFtColour, "/ftcolour", "Cambia el color del texto"},   
};

static t_shell shellData;
void startShell(int argc, char **argv){ 
    shellWelcomeMessage();
    initializeShell();
    shellExecute();
}

void printUser() {
    int len=strlen(shellData.userName);
    len+=4;
    printf("$ ");
    sendUserData(shellData.userName, len);
    printf(" > ");
}

static void initializeShell() {
    strcpy(shellData.userName, "DefaultUser");
    initializeCommands();
}

static void initializeCommands() {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        shellData.commands[i].commandFn = commands[i].commandFn;
        shellData.commands[i].name = commands[i].name;
        shellData.commands[i].description = commands[i].description;
    }
}

static void shellExecute() {
    char command[BUFFER_SIZE] = {0};
    char argv[MAX_ARGUMENTS][BUFFER_SIZE];
    char userInput[BUFFER_SIZE] = {0};
    int argc = 0;
    
    while (1){
        printUser();

        userInput[0] = 0;

        scanf("%s", userInput);
        
        argc = getCommandArgs(userInput, command, argv);

        if(argc == -1) {
            printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n", MAX_ARGUMENTS);
        } 
        int commandIdx = getCommandIdx(command);

        if(commandIdx >= 0) {
            shellData.commands[commandIdx].commandFn(argc, argv);     
        } else {
            printf("\nComando invalido: use /help\n\n");
        }
    }
    return ;
}

static int getCommandIdx(char * command) {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        if ((strcmp(shellData.commands[i].name, command)) == 0) {
            return i;
        }
    }
    return -1;
}



static int getCommandArgs(char* userInput, char* command, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
    int argc = 0;
    int i = 0;
    for(i = 0; userInput[i] != 0 && userInput[i] != ' '; i++) {
        command[i] = userInput[i];
    }
    command[i] = 0;
    int argIdx = 0;

    for(int j = 0; j < MAX_ARGUMENTS; j++) {
            argv[j][0] = 0;
    }

    while(userInput[i] != 0 && argc < MAX_ARGUMENTS) {
        i++;
        argIdx = 0;
        for(; userInput[i] != ' ' && userInput[i] != 0; i++, argIdx++) {
           argv[argc][argIdx] = userInput[i];
        }
        argv[argc][argIdx] = 0;
        argc++;
    }
    if(argc == MAX_ARGUMENTS && userInput[i] != 0)
        return -1;
    return argc;
}

static void shellWelcomeMessage() {
    printf("\n                 Le damos la bienvenida a\n\n");
    logo(0,0);
    printf("\n         Sistemas Operativos --- 2C 2021\n\n");
    printf("\n  Utilice el comando /help para obtener un manual de usuario.\n\n\n\n");
}

static void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 1) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	if(strlen(argv[0]) > USER_SIZE - 1) {
		printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n", USER_SIZE - 1);
		return;
	}
	strcpy(shellData.userName, argv[0]);
	setFirstChange(1);
}

static void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	printf("\nUse ctrl + tab para cambiar de pantalla.\n");	
	printf("\nTabla de colores: \n");
	printf("\nBLANCO | NEGRO | ROJO | VERDE | AZUL\n");
	printf("  1    |   2   |  3   |   4   |  5\n");
	printf("\nLista de comandos: \n");
	for(int i = 0; i < COMMAND_COUNT; i++) {
        printf("%s : %s\n", shellData.commands[i].name, shellData.commands[i].description);
    }
}
