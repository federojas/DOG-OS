#include <RTCID.h>
#include <commands.h>
#include <infoReg.h>
#include <libc.h>
#include <memoryTest.h>
#include <priorityTest.h>
#include <processTest.h>
#include <semaphoreTest.h>
#include <shell.h>
#include <stdint.h>
#include <userSyscalls.h>

#define YEAR                                                                   \
  20 // Debe escribir aca los digitos de su año (excepto los ultimos dos)
#define BYTES 32          // Cantidad de bytes para el mem dump
#define FLOAT_PRECISION 8 // Precision para los puntos flotantes
#define WAIT_SECONDS_LOOP 3


static char* registerNames[] = {
"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
"R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
"RAX: ", "RIP: ", "RSP: "
};

void getCurrentDayTime(int argc, char ** argv) {
	if (checkArgcWrapper(argc, 1) == -1) {
		return;
	}
    printf("\nFecha de hoy: ");
    uint64_t result = _syscall(SYS_RTC_ID, DAY_RTC_ID, 0, 0, 0, 0);
    printf("%d", result);
	putChar('/');

  result = _syscall(SYS_RTC_ID, MONTH_RTC_ID, 0, 0, 0, 0);
  printf("%d", result);
  putChar('/');

  result = _syscall(SYS_RTC_ID, YEAR_RTC_ID, 0, 0, 0, 0);
  printf("%d", YEAR);
  printf("%d\n", result);

  printf("\nHora: ");
  result = _syscall(SYS_RTC_ID, HOUR_RTC_ID, 0, 0, 0, 0);
  printf("%d", result);
  putChar(':');

  result = _syscall(SYS_RTC_ID, MINUTE_RTC_ID, 0, 0, 0, 0);
  if (result < 10) {
    putChar('0');
  }
  printf("%d", result);
  putChar(':');

  result = _syscall(SYS_RTC_ID, SECOND_RTC_ID, 0, 0, 0, 0);
  if (result < 10) {
    putChar('0');
  }
  printf("%d\n\n", result);
}

static void printFeature(uint8_t feature, const char *string) {
  printf("%s", string);
  if (feature != 0) {
    printf(": SI\n");
  } else {
    printf(": NO\n");
  }
}

void getCPUFeatures(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  uint8_t check = _syscall(SYS_CPUID_ID, 0, 0, 0, 0, 0);
  if (check) {
    printf("\nEste procesador soporta CPUID.\n");
  } else {
    printf("\nEste processor no soporta CPUID.\n");
    return;
  }
  uint32_t features[4];
  (void)_syscall(SYS_CPUFEATURES_ID, (uint64_t)features, 0, 0, 0, 0);

  // printf("\nValor de EDX con EAX en 1: %x\n", features[0]);
  // printf("Valor de ECX con EAX en 1: %x\n", features[1]);
  // printf("Valor de EBX con EAX en 7 y ECX en 0: %x\n", features[2]);
  // printf("Valor de ECX con EAX en 7 y ECX en 0: %x\n", features[3]);

  printf("\nCaracteristicas del CPU:\n\n");

  printFeature(features[0] >> 23 & 1, "MMX (Multi-Media Extension)");
  printFeature(features[0] >> 25 & 1, "SSE (Streaming SIMD Extension 1)");
  printFeature(features[0] >> 26 & 1, "SSE2 (Streaming SIMD Extension 2)");
  printFeature(features[1] >> 0 & 1, "SSE3 (Streaming SMD Extension 3)");
  printFeature(features[1] >> 19 & 1, "SSE4.1 (Streaming SIMD Extensions 4.1)");
  printFeature(features[1] >> 20 & 1, "SSE4.2 (Streaming SIMD Extensions 4.2)");
  printFeature(features[1] >> 25 & 1, "AES (Advanced Encryption Standard)");
  printFeature(features[1] >> 1 & 1, "PCLMULQDQ (Carry-less Multiplication)");
  printFeature(features[1] >> 28 & 1, "AVX (Advanced Vector Extensions)");
  printFeature(features[1] >> 29 & 1, "F16C (half-precision) FP feature");
  printFeature(features[1] >> 12 & 1,
               "FMA3 (Fused Multiply-Add 3-operand Form)");

  printf("\nCaracteristicas extendidas del CPU:\n\n");

  printFeature((features[2] >> 5) & 1, "AVX2 (Advanced Vector Extensions 2)");
  printFeature((features[3] >> 10) & 1,
               "VPCLMULQDQ (CLMUL instruction set (VEX-256/EVEX)");
  printFeature((features[3] >> 9) & 1,
               "VAES (Vector AES instruction set (VEX-256/EVEX))");
  newLine();
}

void getInfoReg(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  uint64_t *registers = (uint64_t *)_syscall(SYS_INFOREG_ID, 0, 0, 0, 0, 0);
  newLine();
  for (int i = 0; i < REGISTER_AMOUNT; i++) {
    printf("%s", registerNames[i]);
    printf("%x\n\n", registers[i]);
  }
  newLine();
}

void getMem(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  uint64_t memDir = strToHex(argv[1]);
  if (memDir == -1) {
    printf("\nEl argumento ingresado es invalido. Use /help.\n\n");
    return;
  }

  printf("\nDump de 32 bytes a partir de la direccion: %s\n\n", argv[1]);
  uint8_t buffer[BYTES];
  char print[10];
  (void)_syscall(SYS_PRINTMEM_ID, memDir, (uint64_t)buffer, BYTES, 0, 0);
  for (int i = 0; i < BYTES; i++) {
    if (i == 16) {
      newLine();
    }
    intToStr(buffer[i], print, 16);
    printf("%s ", print);
  }
  newLine();
  newLine();
}

void divZero(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  int x = 3;
  int y = 0;
  x = x / y;
}

// https://mudongliang.github.io/x86/html/file_module_x86_id_318.html
void opCode(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  _opcodeExp();
}

void getRoots(int argc, char **argv) {
  if (checkArgcWrapper(argc, 4) == -1) {
    return;
  }

  double a;
  double b;
  double c;

  strToDouble(argv[1], &a);
  strToDouble(argv[2], &b);
  strToDouble(argv[3], &c);

  double min = 1.0;
  min /= pow(10, FLOAT_PRECISION);

  double aux1;
  double aux2;
  double aux3;

  aux1 = a > 0 ? a : -1 * a;
  aux2 = b > 0 ? b : -1 * b;
  aux3 = c > 0 ? c : -1 * c;

  if (a == 0) {
    printf("\nEl coeficiente de x^2 no puede ser 0.\n\n");
    return;
  }

  if (aux1 < min || (aux2 < min && b != 0) || (aux3 < min && c != 0)) {
    printf("\nLos argumentos pueden tener como maximo %d decimales.\n\n",
           FLOAT_PRECISION);
    return;
  }

  if (b * b - 4 * a * c < 0) {
    printf("\nEl polinomio no puede tener raices imaginarias.\nSu "
           "discriminante no puede ser menor que 0.\n\n");
    return;
  }

  double root1;
  double root2;

  _quadratic(&a, &b, &c, &root1, &root2);

  char res1[BUFFER_SIZE];
  char res2[BUFFER_SIZE];

  doubleToStr(root1, res1, FLOAT_PRECISION);
  doubleToStr(root2, res2, FLOAT_PRECISION);

  printf("\nLas raices son: %s y %s\n\n", res1, res2);
}

void clear(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  (void)_syscall(SYS_CLEAR_ID, 0, 0, 0, 0, 0);
}
void exit(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  printf("\nMuchas gracias por utilizar DOG-OS, esperamos su regreso.\n");
  printf("\nUsted cerro el TP, el mismo ya no funcionara.\n\n");
  logo(1, 0);
  (void)_syscall(SYS_EXIT_ID, 0, 0, 0, 0, 0);
}


// https://patorjk.com/software/taag/#p=display&f=Slant&t=DOG-OS
void logo(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }

    printLogo(WHITE, 0);
}

void doge(int argc, char **argv) {

    if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
    (void)_syscall(SYS_CLEAR_ID, 0, 0, 0, 0, 0);
    // sleep(1);
      int colors[4] = {WHITE, RED, GREEN, BLUE};
  for (int j = 0; j < 40; j++) {
    for (int i = 0; i < 4; i++) {
        printf("\n\n\n\n\n\n\n\n\n\n");
      printLogo(colors[i], 1);
      printf("\n\n\n\n\n");
      printDogAnimation(i);
        sleep(1);
      (void)_syscall(SYS_CLEAR_ID, 0, 0, 0, 0, 0);
    }
  }
}

void getCPUVendor(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  char buffer[9];
  (void)_syscall(SYS_CPUVENDOR_ID, (uint64_t)buffer, 0, 0, 0, 0);
  printf("\nID de fabricante: %s\n\n", buffer);
}

void changeFtColour(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  int aux = strToInt(argv[1], 0);
  switch (aux) {
  case 1:
    setFTC(WHITE);
    break;
  case 2:
    setFTC(BLACK);
    break;
  case 3:
    setFTC(RED);
    break;
  case 4:
    setFTC(GREEN);
    break;
  case 5:
    setFTC(BLUE);
    break;
  default:
    printf("\nEl codigo de color que ingreso es invalido. Use /help.\n\n");
    break;
  }
}

void changeBgColour(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  int aux = strToInt(argv[1], 0);
  switch (aux) {
  case 1:
    setBGC(WHITE);
    break;
  case 2:
    setBGC(BLACK);
    break;
  case 3:
    setBGC(RED);
    break;
  case 4:
    setBGC(GREEN);
    break;
  case 5:
    setBGC(BLUE);
    break;
  default:
    printf("\nEl codigo de color que ingreso es invalido. Use /help.\n\n");
    break;
  }
}

void memStatusWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  memStatus();
}

void semStatusWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  semStatus();
}

void processStatusWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  processStatus();
}

void killProcessWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  int pid = strToInt(argv[1], 0);
  char userInput[2] = {0};
  if (pid < 2) {
    printf("\nUsted va a matar a un proceso clave para el sistema.\nSi quiere "
           "continuar presione y.\n\n");
    scanf("%c", userInput);
    if (userInput[0] != 'y') {
      return;
    }
  }
  killProcess(pid);
}

void setPriorityWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 3) == -1) {
    return;
  }
  int pid = strToInt(argv[1], 0);
  int priority = strToInt(argv[2], 0);
  char userInput[2] = {0};
  if (pid < 2) {
    printf("\nUsted va a cambiar la prioridad de un proceso clave para el "
           "sistema.\nSi quiere continuar presione y.\n\n");
    scanf("%c", userInput);
    if (userInput[0] != 'y') {
      return;
    }
  }
  setPriority(pid, priority);
}

void blockProcessWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  int pid = strToInt(argv[1], 0);
  char userInput[2] = {0};
  if (pid < 2) {
    printf("\nUsted va a bloquear un proceso clave para el sistema.\nSi quiere "
           "continuar presione y.\n\n");
    scanf("%c", userInput);
    if (userInput[0] != 'y') {
      return;
    }
  }
  blockProcess(pid);
}

void unblockProcessWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  int pid = strToInt(argv[1], 0);
  char userInput[2] = {0};
  if (pid < 2) {
    printf("\nUsted va a desbloquear un proceso clave para el sistema.\nSi "
           "quiere continuar presione y.\n\n");
    scanf("%c", userInput);
    userInput[1] = 0;
    if (userInput[0] != 'y') {
      return;
    }
  }
  unblockProcess(pid);
}

void testNoSyncWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  testNoSync();
}

void testSyncWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  testSync();
}

void testProcessesWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  testProcesses();
}
void filter(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }

  char userInput[BUFFER_SIZE];
  char output[BUFFER_SIZE];
  scanf("%s", userInput);
  int inputIndex = 0;
  int outputIndex = 0;
  while (userInput[inputIndex] != 0) {
    if (!isVowel(userInput[inputIndex])) {
      output[outputIndex++] = userInput[inputIndex];
    }
    inputIndex++;
  }
  printf("%s\n", output);
}
void testPriorityWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  testPriority();
}

void testMemoryWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  testMemory();
}

void cat(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  char userInput[BUFFER_SIZE];
  scanf("%s", userInput);
  printf("%s\n", userInput);
}

void loop(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  int pid = getProcessPID();
//   uint32_t timeToWake;
  while (1) {
    printf("El proceso %d dice hola. Imprimiendo devuelta en %d segundos.\n",
           pid, WAIT_SECONDS_LOOP);
    sleep(WAIT_SECONDS_LOOP);
  }
}

void pipeStatusWrapper(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  pipeStatus();
}

void wc(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  char userInput[BUFFER_SIZE];
  scanf("%s", userInput);
  int inputLength = strlen(userInput);
  int halfScreenSize = getHalfScreenSize();
  int lines = (int)((inputLength / halfScreenSize) +
                    ((inputLength % halfScreenSize) != 0));
  printf("\nCantidad de lineas: %d\n\n", lines);
}

int checkArgcWrapper(int argc, int argumentsPermitted) {
  if (argc != argumentsPermitted) {
    printf("\nCantidad invalida de argumentos.\n\n");
    return -1;
  }
  return 0;
}